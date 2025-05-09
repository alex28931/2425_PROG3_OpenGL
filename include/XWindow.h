#include <string>

typedef struct GLFWwindow GLFWwindow;

class XWindow 
{
public:
    XWindow(int InWidth, int InHeight, std::string InTitle);
    ~XWindow();
    bool IsOpened() const;
    void SetTitle(const std::string InTitle);
    void Update();
    float GetDeltaTime() const;
    void SetVSync(bool InEnabled);

private:
    int Width;
    int Height;
    std::string Title;
    GLFWwindow* RawWindow;
    float DeltaTime;
};