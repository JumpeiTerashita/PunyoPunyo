#pragma once


template<class T>
class Sequence {
private:
	typedef void(T::*Logic)();
	Logic sequence;
	int frame;
protected:
	Sequence() {}
	inline void Sequence::runSequence() {
		frame++;
		T* p = static_cast<T*>(this);
		(p->*sequence)();
	}
	
public:
	inline void setSequence(Logic seq) {
		frame = -1;
		sequence = seq;
	}
	virtual ~Sequence() {}
	inline int getFrame() { return frame; }
	inline Logic getSequence() { return sequence; }
	inline void addFrame(int _AddFrame) { frame += _AddFrame; }
	inline void setFrame(int _Frame) { frame = _Frame; }
};
