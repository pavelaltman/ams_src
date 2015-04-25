//---------------------------------------------------------------------------

#ifndef READ_FILEH
#define READ_FILEH

//---------------------------------------------------------------------------
class VReadFile: public Struct {
    std::ifstream File;
    string FileName;
public:
    VReadFile(const string& FileName_);
    virtual ~VReadFile();
    virtual StackItem Method(Node &node,::string const &method,int pc,StackItem *stk);
protected:
};
//-----------------------------------------------------------------------------
#endif
