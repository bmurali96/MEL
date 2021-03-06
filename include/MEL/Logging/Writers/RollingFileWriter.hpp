// MIT License
//
// MEL - Mechatronics Engine & Library
// Copyright (c) 2018 Mechatronics and Haptic Interfaces Lab - Rice University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#ifndef MEL_ROLLINGFILEWRITER_HPP
#define MEL_ROLLINGFILEWRITER_HPP

#include <MEL/Logging/File.hpp>
#include <MEL/Logging/Writers/Writer.hpp>
#include <MEL/Utility/Mutex.hpp>
#include <algorithm>

namespace mel {

//==============================================================================
// CLASS DECLARATION
//==============================================================================

template <class Formatter>
class RollingFileWriter : public Writer {
public:
    RollingFileWriter(const char* filename,
                      size_t max_file_size = 0,
                      int max_files       = 0,
                      Severity max_severity = Debug)
        : Writer(max_severity),
          file_size_(),
          max_file_size_(
              (std::max)(static_cast<off_t>(max_file_size),
                         static_cast<off_t>(
                             1000)))  // set a lower limit for the maxFileSize
          ,
          last_file_number_((std::max)(max_files - 1, 0)),
          first_write_(true) {
        split_file_name(filename, filename_no_ext_, file_ext_);
    }

    virtual void write(const Record& record) {
        Lock lock(mutex_);

        if (first_write_) {
            open_log_file();
            first_write_ = false;
        } else if (last_file_number_ > 0 && file_size_ > max_file_size_ &&
                   -1 != file_size_) {
            roll_log_files();
        }

        int bytes_written =
            file_.write(Formatter::format(record));


        if (bytes_written > 0) {
            file_size_ += bytes_written;
        }
    }

private:
    void roll_log_files() {
        file_.close();

        std::string lastFileName = build_file_name(last_file_number_);
        mel::File::unlink(lastFileName.c_str());

        for (int fileNumber = last_file_number_ - 1; fileNumber >= 0;
             --fileNumber) {
            std::string currentFileName = build_file_name(fileNumber);
            std::string nextFileName    = build_file_name(fileNumber + 1);

            File::rename(currentFileName.c_str(), nextFileName.c_str());
        }

        open_log_file();
    }

    void open_log_file() {
        std::string fileName = build_file_name();
        file_size_           = file_.open(fileName.c_str());

        if (0 == file_size_) {
            int bytesWritten =
                file_.write(Formatter::header());

            if (bytesWritten > 0) {
                file_size_ += bytesWritten;
            }
        }
    }

    std::string build_file_name(int fileNumber = 0) {
        std::ostringstream ss;
        ss << filename_no_ext_;

        if (fileNumber > 0) {
            ss << '.' << fileNumber;
        }

        if (!file_ext_.empty()) {
            ss << '.' << file_ext_;
        }

        return ss.str();
    }

private:
    Mutex mutex_;
    File file_;
    off_t file_size_;
    const off_t max_file_size_;
    const int last_file_number_;
    std::string file_ext_;
    std::string filename_no_ext_;
    bool first_write_;
};
}  // namespace mel

#endif // MEL_ROLLINGFILEWRITER_HPP

//==============================================================================
// CLASS DOCUMENTATION
//==============================================================================
