#include "joysticksdl.h"
extern "C"{
    #include <SDL/SDL.h>
}
JoystickSDL::JoystickSDL()
{

}
void JoystickSDL::run(){
    SDL_Event event;
    SDL_Joystick *js;
    int num_js, i, quit_flag;

    /* Initialize SDL's joystick and video subsystems. */
    if (SDL_Init(SDL_INIT_JOYSTICK  | SDL_INIT_VIDEO) != 0) {
        return;
    }

    atexit(SDL_Quit);
    /* Find out how many joysticks are available. */
    num_js = SDL_NumJoysticks();
    if (num_js == 0) {
        return ;
    }

    /* Print out information about each joystick. */
    for (i = 0; i < num_js; i++) {

        /* Open the joystick. */
        js = SDL_JoystickOpen(i);

        if (js == NULL) {
            qDebug()<<"Unable to open joystick" << i;
        } else {
            qDebug()<<"Joystick "<< i;
            qDebug()<<"Name:       "<<SDL_JoystickName(i);
            qDebug()<<"tAxes:      "<< SDL_JoystickNumAxes(js);
            qDebug()<<"\tTrackballs: "<< SDL_JoystickNumBalls(js);
            qDebug()<<"\tButtons:    "<<SDL_JoystickNumButtons(js);


            SDL_JoystickClose(js);
        }
    }

        /* We'll use the first joystick for the demonstration. */
        js = SDL_JoystickOpen(0);
        if (js == NULL) {
         qDebug()<<"Unable to open joystick: "<< SDL_GetError();
        }

        quit_flag = 0;
        while (SDL_WaitEvent(&event) != 0 ) {
            if(event.type==SDL_JOYAXISMOTION) {
               if(event.jaxis.axis==0)
                   axis0State=event.jaxis.value;
               else if(event.jaxis.axis==1)
                   axis1State=event.jaxis.value;
               else if(event.jaxis.axis==2)
                   axis2State=event.jaxis.value;
               else if(event.jaxis.axis==3)
                   axis3State=event.jaxis.value;
               qDebug()<< "event.jaxis.which: "<< event.jaxis.which <<", event.jaxis.axis:"<< event.jaxis.axis << ",event.jaxis.value" <<event.jaxis.value;
            }
            if(event.type==SDL_JOYBUTTONUP){

                    emit signalButtomPress(false,event.jbutton.button);
                qDebug()<< "event.jbutton.which:"<< event.jbutton.which <<", event.jbutton.button:"<<event.jbutton.button<<", event.jbutton.state:"<< event.jbutton.state;
            }
            if(event.type==SDL_JOYBUTTONDOWN){
                emit signalButtomPress(true,event.jbutton.button);
                qDebug()<< "event.jbutton.which:"<< event.jbutton.which <<", event.jbutton.button:"<<event.jbutton.button<<", event.jbutton.state:"<< event.jbutton.state;
            }

        }

        /* Close the joystick. */
        //SDL_JoystickClose(js);
}


void JoystickSDL::slotSendStatesOfAxisToOpenGlWidget(){
    if(axis1State<0)
        emit signalMoveToFront(((double)axis1State)/(-32768.0)*3);
    if(axis1State>0)
        emit signalMoveToBack(3.0*((double)axis1State)/32768.0);
    if(axis0State>0)
        emit signalMoveToRight(3.0*((double)axis0State)/32768.0);
    if(axis0State<0)
        emit signalMoveToLeft(3.0*((double)axis0State)/(-32768.0));
    if(axis3State!=0)
        emit signalRotateAxis3(-0.2*((double)axis3State)/(32768.0));
    if(axis2State!=0)
        emit signalRotateAxis0(0.05*((double)axis2State)/(32768.0));
}
