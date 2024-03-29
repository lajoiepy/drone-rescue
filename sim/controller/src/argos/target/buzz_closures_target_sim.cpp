#include "buzz_controller_target_sim.h"


namespace buzz_drone_rescue_sim {

/****************************************/
/****************************************/

static int BuzzRandUniform(buzzvm_t vm){

   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzobj_t buzz_range_lowest = buzzvm_stack_at(vm, 2);
   buzzobj_t buzz_range_highest = buzzvm_stack_at(vm, 1);
   float range_lowest, range_highest;

   if(buzz_range_lowest->o.type == BUZZTYPE_FLOAT) range_lowest = buzz_range_lowest->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "uniform(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[buzz_range_lowest->o.type]
         );
      return vm->state;
   } 
   if(buzz_range_highest->o.type == BUZZTYPE_FLOAT) range_highest = buzz_range_highest->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "uniform(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[buzz_range_highest->o.type]
         );
      return vm->state;
   } 

   std::uniform_real_distribution<float> distribution(range_lowest, range_highest);

   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   
   /* Call function */
   float random_value = distribution(
      reinterpret_cast<CBuzzControllerTargetSim*>(buzzvm_stack_at(vm, 1)->u.value)->GetRandomEngine());

   buzzvm_pushf(vm, random_value);

   return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/

static int BuzzRandGauss(buzzvm_t vm){

   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzobj_t buzz_mean = buzzvm_stack_at(vm, 2);
   buzzobj_t buzz_stdev = buzzvm_stack_at(vm, 1);
   int mean, stdev;

   if(buzz_mean->o.type == BUZZTYPE_INT) mean = buzz_mean->i.value;
   else if(buzz_mean->o.type == BUZZTYPE_FLOAT) mean = (int)buzz_mean->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "gauss(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[buzz_mean->o.type]
         );
      return vm->state;
   } 
   if(buzz_stdev->o.type == BUZZTYPE_INT) stdev = buzz_stdev->i.value;
   else if(buzz_stdev->o.type == BUZZTYPE_FLOAT) stdev = (int)buzz_stdev->f.value;
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "gauss(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[buzz_stdev->o.type]
         );
      return vm->state;
   } 
   std::normal_distribution<float> distribution(mean, stdev);

   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   
   /* Call function */
   float random_value = distribution(
      reinterpret_cast<CBuzzControllerTargetSim*>(buzzvm_stack_at(vm, 1)->u.value)->GetRandomEngine());

   buzzvm_pushf(vm, random_value);

   return buzzvm_ret1(vm);
}

/****************************************/
/****************************************/

static int BuzzSetVelocity(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   /* Create a new vector with that */
   CVector2 heading;
   buzzobj_t tX = buzzvm_stack_at(vm, 2);
   buzzobj_t tY = buzzvm_stack_at(vm, 1);
   if(tX->o.type == BUZZTYPE_FLOAT) heading.SetX(tX->f.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "set_velocity(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tX->o.type]
         );
      return vm->state;
   }      
   if(tY->o.type == BUZZTYPE_FLOAT) heading.SetY(tY->f.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "set_velocity(x,y): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_FLOAT],
                      buzztype_desc[tY->o.type]
         );
      return vm->state;
   }
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   
   /* Call function */
   reinterpret_cast<CBuzzControllerTargetSim*>(buzzvm_stack_at(vm, 1)->u.value)->SetVelocity(heading);
   
   return buzzvm_ret0(vm);
}

/****************************************/
/****************************************/

static int BuzzSetLED(buzzvm_t vm) {
   /* Push the vector components */
   buzzvm_lload(vm, 1);
   buzzvm_lload(vm, 2);
   buzzvm_lload(vm, 3);
   /* Create a new vector with that */
   CColor color;
   buzzobj_t tR = buzzvm_stack_at(vm, 3);
   buzzobj_t tG = buzzvm_stack_at(vm, 2);
   buzzobj_t tB = buzzvm_stack_at(vm, 1);
   if(tR->o.type == BUZZTYPE_INT) color.SetRed(tR->i.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "set_velocity(x,y): expected %s, got %s in first argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[tR->o.type]
         );
      return vm->state;
   }      
   if(tG->o.type == BUZZTYPE_INT) color.SetGreen(tG->i.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "set_velocity(x,y): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[tG->o.type]
         );
      return vm->state;
   }
   if(tB->o.type == BUZZTYPE_INT) color.SetBlue(tB->i.value);
   else {
      buzzvm_seterror(vm,
                      BUZZVM_ERROR_TYPE,
                      "set_velocity(x,y): expected %s, got %s in second argument",
                      buzztype_desc[BUZZTYPE_INT],
                      buzztype_desc[tB->o.type]
         );
      return vm->state;
   }
   /* Get pointer to the controller */
   buzzvm_pushs(vm, buzzvm_string_register(vm, "controller", 1));
   buzzvm_gload(vm);
   
   /* Call function */
   reinterpret_cast<CBuzzControllerTargetSim*>(buzzvm_stack_at(vm, 1)->u.value)->SetLEDs(color);
   
   return buzzvm_ret0(vm);
}

/****************************************/
/************ Registration **************/
/****************************************/

buzzvm_state CBuzzControllerTargetSim::RegisterFunctions() {
   CBuzzControllerFootBot::RegisterFunctions();
   
   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "uniform", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzRandUniform));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "gauss", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzRandGauss));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "set_velocity", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetVelocity));
   buzzvm_gstore(m_tBuzzVM);

   buzzvm_pushs(m_tBuzzVM, buzzvm_string_register(m_tBuzzVM, "set_led", 1));
   buzzvm_pushcc(m_tBuzzVM, buzzvm_function_register(m_tBuzzVM, BuzzSetLED));
   buzzvm_gstore(m_tBuzzVM);

   return m_tBuzzVM->state;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CBuzzControllerTargetSim, "buzz_controller_target_sim");

}