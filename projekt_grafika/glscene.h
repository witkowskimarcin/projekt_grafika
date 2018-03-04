#if !defined(_glscene_h)
#define _glscene_h
#include "Tex.h"

class glScene
{
	public:
   	glScene();

      virtual ~glScene();
      //wywolywana gdy rozmiar okna na ekranie ulega zmianie
      virtual void Resize(int, int);
      virtual void Resize();

		//wolana gdy zostanie wcisniety klawisz z klawiatury
      //Kody klawiszy VK_ z naglowka windows.h
      virtual void KeyboardFunction(char, int, int);
      virtual void KeyboardFunction(int, int, int);

      virtual void MouseMove(int, int);
	  virtual void MouseLBDown(int, int);
	  virtual void MouseLBUp(int, int);

      //wywolywana po utworzeniu klasy sceny. Ma za zadanie inicjalizacje OpenGL
      virtual void SetupRC();

      //wolana przed zniszczeniem objektu sceny. Ma za zadanie przywrocic
      //stan poczatkowy OpenGL
      virtual void CleanRC();

      //Wolana przez Timer.
      virtual void Timer();

      //ma zwrocic prawde jesli nasza scena wymaga timera
      virtual bool WantTimer();

      //ma zwrocic liczbe milisekund co ile ma byc wolana metoda Timer()
      virtual int GetTimer();


	  //zwraca stan flagi repaintRequest_ - jesli prawda to OGLmain powinien odmalowac okno
      //Wykozystywane w Idle

	  virtual bool NeedRefresh();

      virtual void RenderScene();

   protected:
		GLfloat rot_[3];

      static GLint winx_, winy_;
	  GLboolean repaintRequest_;
};

inline
bool glScene::NeedRefresh()
{
	return (bool)(repaintRequest_ == GL_TRUE ? true : false);
}


#endif
