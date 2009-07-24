#ifndef GENEFACTORY_H_
#define GENEFACTORY_H_
class Gene;
#include <string>
/**
 * Factory class to produce Genes
 */
class GeneFactory {
public:
	typedef Gene* (*factoryMethod)();
	GeneFactory();
	virtual ~GeneFactory(); //< Destructor. deletes all registered creator functions
	static Gene createRandomGene(); //< \brief returns a random gene of the available registered types
	static void registerGene(factoryMethod, std::string name){
		
	}; //< \brief registers a new Gene with the factory
};

template<class T> class GeneRegistrator {
public:
	GeneRegistrator(std::string name) {
		GeneFactory::registerGene(create,name);
	}

private:
	static Gene * create() {
		return new T( );
	}
};
#endif /*GENEFACTORY_H_*/
