#include "FunctionFactory.h"
#include "Function.h"
void deregister(std::string key){
	//std::cout << "deregistering " << key << std::endl;
	FunctionFactory<double>::instance().dereference(key);
}

static FunctionRegistrator<Constant ,double >  Function_Constant("Constant");
static FunctionRegistrator<BooleanAND,double> Function_BooleanAND("AND");
static FunctionRegistrator<BooleanOR,double> Function_BooleanOR("OR");
static FunctionRegistrator<BooleanNOR,double> Function_BooleanNOR("NOR");
static FunctionRegistrator<BooleanNOT,double> Function_BooleanNOT("NOT");
static FunctionRegistrator<BooleanNAND,double> Function_BooleanNAND("NAND");
static FunctionRegistrator<BooleanXOR,double> Function_BooleanXOR("XOR");
static FunctionRegistrator<Multiply,double> Function_Multiply("*");
static FunctionRegistrator<Divide,double> Function_Divide("/");
static FunctionRegistrator<Subtract,double> Function_Subtract("-");
static FunctionRegistrator<Add,double> Function_Add("+");
static FunctionRegistrator<LeftShift,double> Function_LeftShift("<<");
static FunctionRegistrator<RightShift,double> Function_RightShift(">>");
static FunctionRegistrator<LeftRotate,double> Function_LeftRotate("rotl");
static FunctionRegistrator<SimpleIF,double> Function_SimpleIF("sif");
static FunctionRegistrator<GreaterThan,double> Function_GreaterThan("gt");
static FunctionRegistrator<LessThan,double> Function_LessThan("lt");
static FunctionRegistrator<LogicalIF,double> Function_LogicalIF("lif");
static FunctionRegistrator<Min,double> Function_Min("min");
static FunctionRegistrator<Max,double> Function_Max("max");
static FunctionRegistrator<Sign,double> Function_Sign("sign");
