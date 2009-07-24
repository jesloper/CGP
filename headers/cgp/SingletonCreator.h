#ifndef SINGLETONCREATOR_H_
#define SINGLETONCREATOR_H_

template <class T> struct SingletonCreator {
	SingletonCreator() {
		T::instance();
	}
	inline void dummy() const {
		;
	}
};
#endif /*SINGLETONCREATOR_H_*/
