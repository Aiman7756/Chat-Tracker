#include "ChatTracker.h"

#include <string>
#include <vector>
#include <functional>
#include <list>
using namespace std;

  // This class does the real work of the implementation.

class ChatTrackerImpl
{
  public:
    ChatTrackerImpl(int maxBuckets);
    void join(string user, string chat);
    int terminate(string chat);
    int contribute(string user);
    int leave(string user, string chat);
    int leave(string user);

  private:
    struct Info
    {
        Info(string u, string c) : user(u), chat(c), count(0) {}
        string user;
        string chat;
        int count;
    };
    
    struct Chat{
        Chat(string c) : name(c), count (0) {}
        string name;
        int count;
        };

    vector<list<Info>> info;
    vector<list<Chat>> chats;
    int buckets;
    void initializeChat(string c);
    void chatContribute(string c);
};

ChatTrackerImpl::ChatTrackerImpl(int maxBuckets) : buckets(maxBuckets)
{
    info.resize(buckets);
    chats.resize(buckets);
}


void ChatTrackerImpl::initializeChat(string c){
    
    int i = hash<string>{}(c) % buckets;
    if (chats[i].empty()){
        chats[i].push_back(Chat(c));
        return;
    }
    else{
        list<Chat>::iterator t;
            for (t = chats[i].begin(); t != chats[i].end(); t++){
                if (t->name == c){
                    return;
                }
            }
            chats[i].push_front(Chat(c));
        }
    }


void ChatTrackerImpl::join(string user, string chat)
{
    initializeChat(chat);
    int i = hash<string>{}(user) % buckets;
    list<Info>::iterator t;
    
    if (info[i].size() == 0){
        info[i].push_back(Info(user, chat));
        return;
    }
    else{
        
        for (t = info[i].begin(); t != info[i].end(); t++){
            if (t->user == user && t->chat == chat){
                Info temp = *t;
                t = info[i].erase(t);
                info[i].push_front(temp);
                return;
            }
        }
        info[i].push_front(Info(user, chat));
    }
}

int ChatTrackerImpl::contribute(string user)
{

    int i = hash<string>{}(user) % buckets;
    list<Info>::iterator p;
    
    for (p = info[i].begin(); p != info[i].end(); p++){
        if (p->user == user){
            p->count++;
            chatContribute(p->chat);
            return p->count;
        }
    }
    return 0;
}

void ChatTrackerImpl::chatContribute(string c){
    int i = hash<string>{}(c) % buckets;
    list<Chat>::iterator p;
    for (p = chats[i].begin(); p != chats[i].end(); p++){
        if (p->name == c){
            p->count++;
            return;
        }
    }
}

int ChatTrackerImpl::leave(string user, string chat)
{
    int i = hash<string>{}(user) % buckets;
    
    list<Info>::iterator p;

    
    for (p = info[i].begin(); p != info[i].end(); p++){
        if (p->user == user && p->chat == chat){
            int count = p->count;
             p = info[i].erase(p);
            return count;
        }
    }
    return -1;
}


int ChatTrackerImpl::leave(string user)
{
    int i = hash<string>{}(user) % buckets;
    list<Info>::iterator p;
    for (p = info[i].begin(); p != info[i].end(); p++){
        if (p->user == user){
            int count = p->count;
            p = info[i].erase(p);
            return count;
        }
    }
    return -1;
}

int ChatTrackerImpl::terminate(string chat)
{
    list<Info>::iterator p;
    for (int i = 0; i < info.size(); i++){
        p = info[i].begin();
        while (p != info[i].end()){
            if (p->chat == chat){
                p = info[i].erase(p);
                }
            else {
                p++;
            }
        }
    }
    
    int i = hash<string>{}(chat) % buckets;
    
    list<Chat>::iterator t;
    for (t = chats[i].begin(); t != chats[i].end(); t++){
        if (t->name == chat){
            int cont = t->count;
            t = chats[i].erase(t);
            return cont;
        }
    }
    return 0;
}


//*********** ChatTracker functions **************

// These functions simply delegate to ChatTrackerImpl's functions.
// You probably don't want to change any of this code.

ChatTracker::ChatTracker(int maxBuckets)
{
    m_impl = new ChatTrackerImpl(maxBuckets);
}

ChatTracker::~ChatTracker()
{
    delete m_impl;
}

void ChatTracker::join(string user, string chat)
{
    m_impl->join(user, chat);
}

int ChatTracker::terminate(string chat)
{
    return m_impl->terminate(chat);
}

int ChatTracker::contribute(string user)
{
    return m_impl->contribute(user);
}

int ChatTracker::leave(string user, string chat)
{
    return m_impl->leave(user, chat);
}

int ChatTracker::leave(string user)
{
    return m_impl->leave(user);
}
