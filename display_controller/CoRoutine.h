#ifndef COROUTINE_H
#define COROUTINE_H

class CoRoutine{

private:
	unsigned long previousTime;
	

public:
  float period;

	CoRoutine() {
    previousTime = 0;
  }

  template<typename Func>
	bool execute(Func func){
		unsigned long currentTime = millis();
		unsigned long deltaT = currentTime - previousTime;
    
    if(deltaT >= period){
      previousTime = currentTime;
      return func();
    }
	}

};

#endif