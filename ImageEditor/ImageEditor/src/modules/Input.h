#ifndef __INPUT_H__
#define __INPUT_H__

class Input
{
public:
	Input();
	~Input();

	void Start();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void CleanUp();
};

#endif /* __INPUT_H__ */