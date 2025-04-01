#ifndef DRot_H
#define DRot_H
#include <stdint.h>
class DRot
{
public:


  DRot();
    
  // Public API

  void Init();
  void Turn();
	int Get(); // grabs value on encoder
	void Zero(); // zeros out value on rotary encoder


private:
	int last_state;
	int current_state;
	int incr;
  const static int INPUT_MASK = 0x30;
};

// Every user of the DRot driver class will get this when they include DRot.h.
// It tells the linker that there is an instance of DRot called g_DRot---the one and
// only instance of the DRot driver, typically instantiated at the top of main.cpp.
//
// This means you must use "g_DRot" as the name of your global DRot instance... or
// change both this name and the global variable definition in main.cpp.
extern DRot g_DRot;

#endif  // DRot_H