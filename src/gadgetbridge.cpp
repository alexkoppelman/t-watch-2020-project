#include "config.h"
#include <Arduino.h>
#include "ArduinoJson.h"
#include "gui.h"
#include "main.h"
// #include <string>
// using namespace std;

StaticJsonDocument<512> json;
static MBox *mbox = nullptr;
unsigned long notify_id = 0;
// creating message queue


// void copyToQueue() {
//     char* MessageQueue[3][20] = {"1","2","3"}; 
    
//     // Serial.println("MessageQueue:");
//     // Serial.println(MessageQueue[0]);
//     // Serial.println(MessageQueue[1]);
//     // Serial.println(MessageQueue[2]);
//     char* reducedBody = json["body"];
//     //MessageQueue[0] = (char*)reducedBody;

// }

void process_gadgetbridge_notify() {
    notify_id = json["id"];
    char format[512+3];
    const char* src = json["src"]; // Debug sends "sender"
    const char* title = json["title"]; // Debug sends "subject"
    const char* body = json["body"];
    TTGOClass *ttgo = TTGOClass::getWatch();
    
    snprintf(format, sizeof(format), "%s: %s\n\n%s", src, title, body);
    // copyToQueue();
    // add message to message queue to be able to display last 3 msg
    // if queue is 1 2 3  - 3 being last
    
    delete mbox;
    mbox = new MBox;
    mbox->create(format, [](lv_obj_t *obj, lv_event_t event) {
        if (event == LV_EVENT_VALUE_CHANGED) {
            delete mbox;
            mbox = nullptr;
            notify_id = 0;
        }
    });

    // Turn on display if off
    if (!ttgo->bl->isOn()) {
        xEventGroupSetBits(*get_isr_group(), WATCH_FLAG_SLEEP_EXIT);
    }

    // Trigger vibration
    ttgo->motor->adjust(255);
    ttgo->motor->onec();
}

void process_gadgetbridge_json(const char* json_string) {
    deserializeJson(json, json_string);

    const char* t = json["t"];
    if (!strcmp(t, "notify")) {
        process_gadgetbridge_notify();
        } else if (!strcmp(t, "musicinfo")) {
                const char* artist = json["artist"]; // Debug sends "artist"
                const char* album = json["album"]; // Debug sends "album"
                Serial.println("album and artist:");
                Serial.println(album);
                Serial.println(artist);
         } else if (!strcmp(t, "musicstate")){  
                
                const char* musicstate = json["state"]; // Debug sends "music player state"
                Serial.println("Music state changed to:");
                Serial.println(musicstate);
                
              }  else if (!strcmp(t, "call")){
                char format[512+3];
                const char* callcmd = json["cmd"]; // Debug sends "call command"
                const char* callname = json["name"]; // Debug sends "name of caller"
                const char* callnumber = json["number"]; // Debug sends "number calling"
                TTGOClass *ttgo = TTGOClass::getWatch();
                // vibrate if call recieved
                Serial.println("Call recieved:");
                Serial.println(callname);
                Serial.println(callnumber);
                snprintf(format, sizeof(format), "%s: %s\n\n%s", callcmd, callname, callnumber);
                delete mbox;
                mbox = new MBox;
                mbox->create(format, [](lv_obj_t *obj, lv_event_t event) {
                    if (event == LV_EVENT_VALUE_CHANGED) {
                        delete mbox;
                        mbox = nullptr;
                        //notify_id = 0;
                    }
                });
                if (!ttgo->bl->isOn()) {
                    xEventGroupSetBits(*get_isr_group(), WATCH_FLAG_SLEEP_EXIT);
                }
                ttgo->motor->adjust(255);
                ttgo->motor->onec();

    } else {
        Serial.printf("Unhandled GB type: %s\n", t);
    }
}
