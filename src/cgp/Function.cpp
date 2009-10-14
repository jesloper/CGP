#include "FunctionFactory.h"
#include "Function.h"
typedef double CGPBaseType;
void deregister(QString key){
	//std::cout << "deregistering " << key << std::endl;
        FunctionFactory<CGPBaseType>::instance().dereference(key);
}

static FunctionRegistrator<Constant ,CGPBaseType >  Function_Constant("Constant");
static FunctionRegistrator<BooleanAND,CGPBaseType> Function_BooleanAND("and");
static FunctionRegistrator<BooleanOR,CGPBaseType> Function_BooleanOR("or");
static FunctionRegistrator<BooleanNOR,CGPBaseType> Function_BooleanNOR("nor");
static FunctionRegistrator<BooleanNOT,CGPBaseType> Function_BooleanNOT("not");
static FunctionRegistrator<BooleanNAND,CGPBaseType> Function_BooleanNAND("nand");
static FunctionRegistrator<BooleanXOR,CGPBaseType> Function_BooleanXOR("xor");
static FunctionRegistrator<Multiply,CGPBaseType> Function_Multiply("*");
static FunctionRegistrator<Divide,CGPBaseType> Function_Divide("/");
static FunctionRegistrator<Subtract,CGPBaseType> Function_Subtract("-");
static FunctionRegistrator<Add,CGPBaseType> Function_Add("+");
static FunctionRegistrator<LeftShift,CGPBaseType> Function_LeftShift("<<");
static FunctionRegistrator<RightShift,CGPBaseType> Function_RightShift(">>");
static FunctionRegistrator<LeftRotate,CGPBaseType> Function_LeftRotate("rotl");
static FunctionRegistrator<SimpleIF,CGPBaseType> Function_SimpleIF("sif");
static FunctionRegistrator<GreaterThan,CGPBaseType> Function_GreaterThan("gt");
static FunctionRegistrator<Double,CGPBaseType> Function_Double("double");
static FunctionRegistrator<LessThan,CGPBaseType> Function_LessThan("lt");
static FunctionRegistrator<LogicalIF,CGPBaseType> Function_LogicalIF("lif");
static FunctionRegistrator<Min,CGPBaseType> Function_Min("min");
static FunctionRegistrator<Max,CGPBaseType> Function_Max("max");
static FunctionRegistrator<Sign,CGPBaseType> Function_Sign("sign");
