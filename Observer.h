#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class IObserver 
{
public:
    virtual ~IObserver() {};
    virtual void Update(const std::string& message_from_subject) = 0;
};

class ISubject 
{
public:
    virtual ~ISubject() {};
    virtual void Attach(IObserver* observer) = 0;
    virtual void Detach(IObserver* observer) = 0;
    virtual void Notify() = 0;
};


class Subject : public ISubject 
{
public:
    virtual ~Subject() {
        std::cout << "Goodbye, I was the Subject.\n";
    }

    void Attach(IObserver* observer) override 
    {
        m_vecObserver.push_back(observer);
    }

    void Detach(IObserver* observer) override 
    {
        // remove는 특정 값을 찾아서 완전히 지우는 것이 아닌, 
        // 벡터의 뒤로 보내기 때문에 추가로 erase를 해줘야 한다.
        m_vecObserver.erase(remove(m_vecObserver.begin(), m_vecObserver.end(), observer), m_vecObserver.end());
    }

    void Notify() override 
    {
        std::vector<IObserver*>::iterator iterator = m_vecObserver.begin();
        
        HowManyObserver();
        
        while (iterator != m_vecObserver.end()) 
        {
            (*iterator)->Update(m_strMessage);
            ++iterator;
        }
    }

    void CreateMessage(std::string message = "Empty") 
    {
        this->m_strMessage = message;
        Notify();
    }

    void HowManyObserver() 
    {
        std::cout << "There are " << m_vecObserver.size() << " observers in the vector.\n";
    }

    void SomeBusinessLogic() 
    {
        this->m_strMessage = "change message message";
        Notify();
        std::cout << "I'm about to do some thing important\n";
    }

private:
    std::vector<IObserver*> m_vecObserver;
    std::string m_strMessage;
};

class Observer : public IObserver 
{
public:
    Observer(Subject& subject) : m_subject(subject) 
    {
        this->m_subject.Attach(this);
        std::cout << "Hi, I'm the Observer \"" << ++Observer::m_staticNumber << "\".\n";
        this->m_nNumber = Observer::m_staticNumber;
    }

    virtual ~Observer() 
    {
        std::cout << "Goodbye, I was the Observer \"" << this->m_nNumber << "\".\n";
    }

    void Update(const std::string& message_from_subject) override 
    {
        m_strMessageFromSubject = message_from_subject;
        PrintInfo();
    }

    void RemoveMeFromTheVector() 
    {
        m_subject.Detach(this);
        std::cout << "Observer \"" << m_nNumber << "\" removed from the vector.\n";
    }

    void PrintInfo() 
    {
        std::cout << "Observer \"" << this->m_nNumber << "\": a new message is available --> " << this->m_strMessageFromSubject << "\n";
    }

private:
    std::string m_strMessageFromSubject;
    Subject& m_subject;
    static int m_staticNumber;
    int m_nNumber;
};

int Observer::m_staticNumber = 0;

void ClientCode() {
    Subject* subject = new Subject;
    Observer* observer1 = new Observer(*subject);
    Observer* observer2 = new Observer(*subject);
    Observer* observer3 = new Observer(*subject);
    Observer* observer4;
    Observer* observer5;

    subject->CreateMessage("Hello World! :D");
    observer3->RemoveMeFromTheVector();

    subject->CreateMessage("The weather is hot today! :p");
    observer4 = new Observer(*subject);

    observer2->RemoveMeFromTheVector();
    observer5 = new Observer(*subject);

    subject->CreateMessage("My new car is great! ;)");
    observer5->RemoveMeFromTheVector();

    observer4->RemoveMeFromTheVector();
    observer1->RemoveMeFromTheVector();

    delete observer5;
    delete observer4;
    delete observer3;
    delete observer2;
    delete observer1;
    delete subject;
}