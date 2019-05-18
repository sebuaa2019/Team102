#ifndef VOICE_H_INCLUDED
#define VOICE_H_INCLUDED

void InitKeyword();
void KeywordCB(const std_msgs::String::ConstPtr & msg);
static void FollowSwitch(bool inActive, float inDist);
void Speak(string inStr);
void AddNewWaypoint(string inStr);
static string FindKeyword(string inSentence);
void InitKeyword();

#endif // VOICE_H_INCLUDED
