#ifndef FUNCTION_H
#define FUNCTION_H
#include <list>
#include <stdlib.h>
#include <string>
#include <qdebug.h>
#include <sstream>
#include "MathFunctions.h"
#include <math.h>

void deregister(std::string);
/**
 * Converts a number to string
 */
template<typename T> std::string NumToString(T source) {
    std::ostringstream buffer;
    buffer << source;
    return buffer.str();
}

/**
 * \brief Base class for all functions
 */
template<class T> class Function {
public:
    Function() {
    }
    ;
    virtual ~Function() {
        deregister(m_name);
    }
    ;

    /**
         *  \return the name of the function
         */
    virtual std::string name() {
        return m_name;
    }

    /**
         * \return a description of the function
         */
    virtual std::string description() {
        return m_description;
    }

    /**
         *  \return the number of inputs for this function
         */
    virtual int numberOfInputs() {
        return m_inputs;
    }
    ;
    virtual void copy(Function<T>* other) {

    }
    virtual T calculate(T* inputs) = 0;

    std::string toStdString() {
        std::ostringstream str;
        str << "Name: " << m_name << std::endl << "Description: "
                << m_description << std::endl << "Num inputs : " << m_inputs
                << std::endl;
        return str.str();
    }
    virtual QString toMatlabCode(QString& inp1, QString& inp2) {
        return QString("%1(%2,%3);").arg(m_name.c_str()).arg(inp1).arg(inp2);
    }

protected:
    int m_inputs;
    std::string m_name;
    std::string m_description;
};

template<class T> class BinaryOperator: public Function<T> {
    virtual T calculate(T* inputs) = 0;
    virtual QString toMatlabCode(QString& inp1, QString& inp2) {
        return QString("(%1 %2 %3);").arg(inp1).arg(Function<T>::m_name.c_str()).arg(
                inp2);
    }
};
template<class T> class UnaryOperator: public Function<T> {
    virtual T calculate(T* inputs) = 0;
    virtual QString toMatlabCode(QString& inp1, QString& inp2) {
        return QString("%1(%2);").arg(Function<T>::m_name.c_str()).arg(inp1);
    }
};
/**
 * \brief Boolean AND function
 */
template<class T> class BooleanAND: public Function<T> {
public:
    BooleanAND() {
        Function<T>::m_name = "and";
        Function<T>::m_description
                = "Logical AND operation.  Will return 1 if both inputs are > 0. Otherwise 0.";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        bool i1 = (inputs[0] != 0);
        bool i2 = (inputs[1] != 0);
        return i1 && i2;
    }
};

/**
 * \brief Boolean OR function
 */
template<class T> class BooleanOR: public Function<T> {
public:
    BooleanOR() {
        Function<T>::m_name = "or";
        Function<T>::m_description
                = "Logical OR operation.  Will return 1 if either inputs are > 0. Otherwise 0.";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        bool i1 = (inputs[0] != 0);
        bool i2 = (inputs[1] != 0);
        return i1 || i2;
    }
};
/**
 * \brief Boolean NOR function
 */
template<class T> class BooleanNOR: public Function<T> {
public:
    BooleanNOR() {
        Function<T>::m_name = "nor";
        Function<T>::m_description = "Logical NOR operation.  simply a !OR.";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        bool i1 = (inputs[0] != 0);
        bool i2 = (inputs[1] != 0);
        return !(i1 || i2);
    }
};

/**
 * \brief Boolean NOT function
 */
template<class T> class BooleanNOT: public Function<T> {
public:
    BooleanNOT() {
        Function<T>::m_name = "not";
        Function<T>::m_description
                = "Logical NOT operation.  Will return 0 if input is > 0. Otherwise 1.";
        Function<T>::m_inputs = 1;
    }
    T calculate(T* inputs) {
        bool i1 = (inputs[0] != 0);
        return !i1;
    }
};
/**
 * \brief Boolean NAND function
 */
template<class T> class BooleanNAND: public Function<T> {
public:
    BooleanNAND() {
        Function<T>::m_name = "nand";
        Function<T>::m_description
                = "Logical NAND operation.  Will return 1 if both inputs are <= 0. Otherwise 0.";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        return !((bool) inputs[0] && (bool) inputs[1]);
    }
};

/**
 * \brief Boolean XOR function
 */
template<class T> class BooleanXOR: public Function<T> {
public:
    BooleanXOR() {
        Function<T>::m_name = "xor";
        Function<T>::m_description
                = "Logical XOR operation.  Will return 1 exactely one of the inputs are >= 0. Otherwise 0.";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        bool i1 = (inputs[0] != 0);
        bool i2 = (inputs[1] != 0);
        return (i1 ^ i2);
    }
};
/**
 * \brief multiplication function
 */
template<class T> class Multiply: public BinaryOperator<T> {
public:
    Multiply() {
        Function<T>::m_name = "*";
        Function<T>::m_description = "Multiplies two input numbers";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        return inputs[0] * inputs[1];
    }
};

template<class T> class Add: public BinaryOperator<T> {
public:
    Add() {
        Function<T>::m_name = "+";
        Function<T>::m_description = "Adds two input numbers";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        return inputs[0] + inputs[1];
    }
};

template<class T> class Subtract: public BinaryOperator<T> {
public:
    Subtract() {
        Function<T>::m_name = "-";
        Function<T>::m_description = "Subtracts two input numbers";
        Function<T>::m_inputs = 2;
    }

    T calculate(T* inputs) {
        return inputs[0] - inputs[1];
    }
};

template<class T> class Divide: public BinaryOperator<T> {
public:
    Divide() {
        Function<T>::m_name = "/";
        Function<T>::m_description
                = "Divides two input numbers.  If dividend = 0 then function returns 0";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        if (fabs(inputs[0] - inputs[1]) <= 0.000001) {
            return 1;
        }
        if (fabs(inputs[1]) <= 0.0000001) {
            return inputs[0]; //If divisor is very close to 0 we return first value
        }
        return inputs[0] / inputs[1];
    }
    virtual QString toMatlabCode(QString& inp1, QString& inp2) {
        return QString("divide(%1,%2);").arg(inp1).arg(inp2);
    }
};

template<class T> class LeftShift: public Function<T> {
public:
    LeftShift() {
        Function<T>::m_name = "<<";
        Function<T>::m_description = "Shifts first input one to the left";
        Function<T>::m_inputs = 1;
    }
    T calculate(T* inputs) {
        //int num = (int)inputs[0];
        return (int) inputs[0] << 1;
    }
    virtual QString toMatlabCode(QString& inp1, QString& inp2) {
        return QString("leftshift(%1);").arg(inp1);
    }
};

template<class T> class RightShift: public Function<T> {
public:
    RightShift() {
        Function<T>::m_name = ">>";
        Function<T>::m_description = "Shifts first input one to the right";
        Function<T>::m_inputs = 1;
    }
    T calculate(T* inputs) {
        //int num = (int)inputs[0];
        return (int) inputs[0] >> 1;
    }
    virtual QString toMatlabCode(QString& inp1, QString& inp2) {
        return QString("rightshift(%1);").arg(inp1);
    }
};

template<class T> class LeftRotate: public Function<T> {
public:
    LeftRotate() {
        Function<T>::m_name = "rotl";
        Function<T>::m_description = "Rotates the first input one to the left";
        Function<T>::m_inputs = 1;
    }
    T calculate(T* inputs) {
        unsigned int top_bit = (unsigned int) inputs[0] >> 31; //keep the topmost bit
        unsigned int value = (unsigned int) inputs[0] << 1; //shift one to the left
        value |= top_bit;

        return value;
    }
    virtual QString toMatlabCode(QString& inp1, QString& inp2) {
        return QString("leftrotate(%1);").arg(inp1);
    }
};

template<class T> class SimpleIF: public Function<T> {
public:
    SimpleIF() {
        Function<T>::m_name = "sif";
        Function<T>::m_description
                = "returns second input if first input is true (>0) else it returns 0;";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        if (inputs[0] > 0) {
            return inputs[1];
        }
        return 0;
    }
};
template<class T> class LogicalIF: public UnaryOperator<T> {
public:
    LogicalIF() {
        Function<T>::m_name = "lif";
        Function<T>::m_description
                = "returns 1 if first input > 0. else it returns 0;";
        Function<T>::m_inputs = 1;
    }
    T calculate(T* inputs) {
        if (inputs[0] > 0) {
            return 1;
        }
        return 0;
    }
};

template<class T> class GreaterThan: public Function<T> {
public:
    GreaterThan() {
        Function<T>::m_name = "gt";
        Function<T>::m_description
                = "If first input is larger than second then return 1. otherwise return 0.";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        if (inputs[0] > inputs[1]) {
            return 1;
        }
        return 0;
    }
};
template<class T> class LessThan: public Function<T> {
public:
    LessThan() {
        Function<T>::m_name = "lt";
        Function<T>::m_description
                = "If first input is less than second then return 1. otherwise return 0.";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        if (inputs[0] < inputs[1]) {
            return 1;
        }
        return 0;
    }
};

template<class T> class Min: public Function<T> {
public:
    Min() {
        Function<T>::m_name = "min";
        Function<T>::m_description = "The minimum of up to 3 inputs";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        if (inputs[0] < inputs[1]) {
            return inputs[0];
        }
        return inputs[1];
    }
};
template<class T>
        class Max: public Function<T> {
public:
    Max() {
        Function<T>::m_name = "max";
        Function<T>::m_description = "returns the max of 3 inputs";
        Function<T>::m_inputs = 2;
    }
    T calculate(T* inputs) {
        if (inputs[0] > inputs[1]) {
            return inputs[0];
        }
        return inputs[1];
    }
};

template<class T>
        class Sign: public UnaryOperator<T> {
public:
    Sign() {
        Function<T>::m_name = "sign";
        Function<T>::m_description
                = "If first input is less than second then return 1. otherwise return 0.";
        Function<T>::m_inputs = 1;
    }
    T calculate(T* inputs) {
        if (inputs[0] > 0) {
            return 1;
        } else if (inputs[0] == 0) {
            return 0;
        } else {
            return -1;
        }
    }
};
template<class T> class Constant: public Function<T> {
public:
    //double constant;
    Constant() {

        Function<T>::m_name = "Constant";
        Function<T>::m_description = "Constant number between 0 and 1";
        Function<T>::m_inputs = 0;
        m_constant = getRandInt(0, 100) / 100.0;
    }
    virtual void copy(Function<T>* other) {
        m_constant = static_cast<Constant*> (other)->m_constant;
    }
    T calculate(T* = NULL) {
        return m_constant;
    }
    virtual QString toMatlabCode(QString& inp1, QString& inp2) {
        return QString("%1;").arg(m_constant);
    }
private:
    T m_constant;
};

/**
 * 	returns the set of default functions always available
 */
template<class T> static void getFunctions(std::list<Function<T>*>& list) {
    list.push_back(new BooleanAND<T> );
    list.push_back(new BooleanOR<T> );
    list.push_back(new BooleanNOR<T> );
    list.push_back(new BooleanNOT<T> );
    list.push_back(new BooleanNAND<T> );
    list.push_back(new BooleanXOR<T> );
    list.push_back(new Multiply<T> );
    list.push_back(new Divide<T> );
    list.push_back(new Subtract<T> );
    list.push_back(new Add<T> );
    list.push_back(new LeftShift<T> );
    list.push_back(new RightShift<T> );
    list.push_back(new LeftRotate<T> );
    list.push_back(new SimpleIF<T> );
    list.push_back(new LogicalIF<T> );
    list.push_back(new GreaterThan<T> );
    list.push_back(new LessThan<T> );
    list.push_back(new Constant<T> );
}

#endif
