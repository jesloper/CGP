#ifndef FUNCTIONFACTORY_H_
#define FUNCTIONFACTORY_H_

#include "Function.h"
#include "MathFunctions.h"
#include <string>
#include <map>
#include "SingletonCreator.h"
#include <iostream>
/**
 * Factory class to produce Functions
 */
template<class T>
class FunctionFactory {
public:
	typedef Function<T>* (*factoryMethod)(); ///< \brief typedef method returning Function object
	typedef std::map<std::string, factoryMethod> FactoryMap; ///< \brief map of factory methods
	typedef std::map<std::string, int> ReferenceMap; ///< \brief counts the number of references

	virtual ~FunctionFactory() {
	}
        //< Destructor. deletes all registered creator functions
	static FunctionFactory<T>& instance(); ///< \brief access to singleton instance
	Function<T>* createRandomFunction(); //< \brief returns a random function of the available registered types

	/**
	 * Registers a new function with the factory.
	 * If it already exists it is overwritten.
	 * \param m factory method (returning object of class Function<T>)
	 * \param name string. Name of the function to be put in factory.
	 */
	void registerFunction(factoryMethod m, std::string name) {
		m_function_map[name] = m;
		m_reference_map[name] = 0;
	}

	/**
	 * Creates and returns the function for the given key. If the key is not recognized NULL is returned.
	 * \return pointer to a Function<T> object. Can be NULL.
	 */
	Function<T>* create(std::string key) {
		typename FactoryMap::const_iterator it = m_function_map.find(key); //must use typename keyword due to ambiguity

		if (it != m_function_map.end()) {
			m_reference_map[key]++;
			//std::cout << "Created function. New value for '" << key << "' is "
			//		<< m_reference_map[key] << std::endl;
			return it->second();

		}

		return NULL;
	}
	Function<T>* createRandom() {
			int random = getRandInt(0,getAllFunctions().size()-1);
			std::cout << "Random is " << random;
			typename FactoryMap::const_iterator it = m_function_map.begin(); //must use typename keyword due to ambiguity
			for(int i =0; i < random;i++){
				it++;
			}
			std::cout << "creating random function " << it->first << std::endl;
			return it->second();

		}
	void dereference(std::string key) {
		m_reference_map[key]--;
		//std::cout << "Dereferenced function. New value for '" << key << "' is " << m_reference_map[key]<< std::endl;
	}

	/**
	 * Access to all function stored in the factory
	 * \return vector of strings with function names
	 */
	std::vector<std::string> getAllFunctions() {
		std::vector<std::string> functions;
		typename FactoryMap::iterator it = m_function_map.begin();
		for (; it != m_function_map.end(); it++) {
			functions.push_back((*it).first);
		}
		return functions;
	}

	/**
	 * Prints all functions stored in the map
	 */
	void print() {
		typename FactoryMap::iterator it = m_function_map.begin();
		for (; it != m_function_map.end(); it++) {
			std::cout << "Function: " << (*it).first.c_str() << std::endl;
		}
	}
private:
	FunctionFactory() {
	}
        ///< \brief private due to singleton
	FactoryMap m_function_map; ///< \brief the local map of factory methods
	ReferenceMap m_reference_map;
};

/**
 * Access to singleton
 */
template<class T>
FunctionFactory<T>& FunctionFactory<T>::instance() {
	static FunctionFactory<T>* instance = new FunctionFactory<T> ();
	return *instance;
}

/**
 * this class is used to simplify registration of new Function objects.
 * Example:
 * \code
 * template <class T>
 * class myFunction : public Function<T>{
 * T calculate(T* inp){
 * 	return inp[0];
 * }
 * };
 * static FunctionRegistrator<myFunction,double> myFunction_instance("MyFunction");
 * \endcode
 */
template<template<typename > class T, typename R> class FunctionRegistrator {
public:
	FunctionRegistrator(std::string name); ///< \brief ctor.

private:
	static Function<R> * create(); /// \brief actual function method
};

/**
 * Ctor. Registers the create method with the FunctionFactory
 * \param name name of the function to be registered
 */
template<template<typename > class T, typename R>
FunctionRegistrator<T, R>::FunctionRegistrator(std::string name) {
	FunctionFactory<R>::instance().registerFunction(create, name);
}
/**
 * Creates a new Function object.
 * \return pointer to Function<R> object
 */
template<template<typename > class T, typename R>
Function<R> * FunctionRegistrator<T, R>::create() {
	return new T<R> ();
}
#endif /*FUNCTIONFACTORY_H_*/

