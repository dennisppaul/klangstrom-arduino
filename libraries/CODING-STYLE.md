# Klangstrom Libraries Coding Style

==@TODO(update BSP function naming)==
==@TODO(update C-style implementation)==
==@TODO(remove "Callback Dispatcher Pattern" and classes)==

- favor `#pragma once`  over classic include guard in header files 
- parameters in function and method calls are written in lower case with underscores replacing space e.g: `void foobar(float bar_value);`

## Class Definitions

- use C functions ( with `extern "C" {...}` ) for board-specific (BSP) implementations
- preface BSP functions with `KLST_BSP_`
- implement `init(...)` method to allow deferred initialization 

```
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void KLST_BSP_myperipheral_init();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class MyPeripheral {
public:
    MyPeripheral();
    virtual ~MyPeripheral();

    void init();
};
#endif // __cplusplus
```

```
MyPeripheral::MyPeripheral() {
}

MyPeripheral::~MyPeripheral() {
}

void MyPeripheral::init() {
    KLST_BSP_myperipheral_init();
}
```

## Callback Dispatcher Pattern

classes that can occur as multiple peripherals ( i.e audio codecs or serial ports ) may implement the callback dispatcher pattern:

`CLib.h`:

```
#pragma once

extern "C" {
    typedef void (*C_Callback)(int id, int event);
        
    void register_callback(C_Callback cb);
    void fire_callback(int id, int payload);
    void c_lib_fires_event();
}
```

`CLib.cpp`:

```
#include <stdio.h>
#include "CLib.h"
#include "MyClass.h"

extern "C" {
    static C_Callback fCallback = MyClass::static_callback;

    void fire_callback(int id, int payload) {
        if (fCallback) {
            fCallback(id, payload);
        } else {
            printf("ERROR: callback not registered\n\r");
        }
    }
    
	void c_lib_fires_event() {
        fire_callback(0, 23);
        fire_callback(1, 42);
	}
}

```

`MyClass.h`:

```
#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <mutex>

#include "CLib.h"

class MyClass {
public:
    MyClass() {
        fID = fIDCounter++;
        {
            std::lock_guard<std::mutex> lock(instances_mutex);
            if (instances.size() <= fID) {
                instances.resize(fID + 1, nullptr);
            }
            instances[fID] = this;
        }
    }

    ~MyClass() {
        std::lock_guard<std::mutex> lock(instances_mutex);
        instances[fID] = nullptr;
        std::cout << "removing instance " << fID << std::endl;
    }

    static void static_callback(int id, int event) {
        std::lock_guard<std::mutex> lock(instances_mutex);
        if (id >= 0 && id < instances.size() && instances[id]) {
            instances[id]->handle_event(event);
        } else {
            std::cout << "could not find id: " << id << std::endl;
        }
    }

private:
    static int fIDCounter;
    static std::vector<MyClass*> instances;
    static std::mutex instances_mutex;

    int fID;

    void handle_event(int event) {
        std::cout << "instance " << fID << " received event: " << event << std::endl;
    }
};
```

`MyClass.cpp`:

```
#include "MyClass.h"

int MyClass::fIDCounter = 0;
std::vector<MyClass*> MyClass::instances;
std::mutex MyClass::instances_mutex;
```

`main.cpp`:

```
#include <iostream>
#include "CLib.h"
#include "MyClass.h"

MyClass* obj;

extern "C" {

int main() {
    fire_callback(2, 123); // fire non-existing object

    MyClass obj0{};
    MyClass obj1{};
    MyClass obj2{};
    obj = new MyClass();

    c_lib_fires_event();
    fire_callback(2, 123);
    fire_callback(5, -1); // call out of bounds
    fire_callback(3, 11); // call new`d object
    delete obj;
    fire_callback(3, 22); // trying to call invalid slot
    
    return 0;
}
```

compile with `‌g++ main.cpp CLib.cpp MyClass.cpp -std=c++11 -I . -o main ; ./main`