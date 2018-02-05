#include <MEL/Utility/Console.hpp>

using namespace mel;

// Method 1: Member function
class MyClass {
public:
    // function and data affected by ctrl+c handler must be static
    static bool class_block;
    static int class_handler(int param) {
        class_block = false;
        return 1;
    }
};

bool MyClass::class_block(true); // must define static member data outside class

// Method 2: Global function
static bool global_block = true;
static int global_handler(int param) {
    print("Ctrl-C Pressed");
    print(param);
    global_block = false;
    return 1;
}

int main(int argc, char *argv[]) {
    print("Press Ctrl+C to continue.");
    // Method 1
    register_ctrl_c_handler(MyClass::class_handler);
    while (MyClass::class_block) {}
    print("Since we made it this far, the class Ctrl+C handler was invoked.");
    // Method 2
    register_ctrl_c_handler(global_handler);
    while (global_block) {}
    print("Since we made it this far, the global Ctrl+C handler was invoked.");
}
