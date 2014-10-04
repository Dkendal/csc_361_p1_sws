#include <string>

class Request
{
  public:
    Request(std::string, std::string, std::string);
    bool IsValid();

  private:
    std::string verb, resource, protocol;
    bool IsVerbValid();
};
