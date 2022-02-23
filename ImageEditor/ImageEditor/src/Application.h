#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Application
{
public:
    Application();
    ~Application();

    void Start();

    void PreUpdate();
    void Update();
    void PostUpdate();

    void CleanUp();
};

#endif /* __APPLICATION_H__ */