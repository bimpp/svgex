#include <bimsvg/bimsvg.hpp>

#include <fstream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    std::ifstream ifs(argv[1], std::ifstream::in);
    if (!ifs.good())
    {
        return 1;
    }

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    typedef bimsvg<double> bimsvg_d;
    bimsvg_d::plan plan;
    bimsvg_d::load_from_string(str, plan);
    return 0;
}
