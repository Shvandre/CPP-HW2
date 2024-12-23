#include <bits/stdc++.h>
#include "FluidCalc.tpp"
#include "types.tpp"

constexpr size_t N = 36, M = 84;
constexpr size_t T = 1'000'000;
char field[N][M + 1] = {
    "####################################################################################",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                       .........                                  #",
    "#..............#            #           .........                                  #",
    "#..............#            #           .........                                  #",
    "#..............#            #           .........                                  #",
    "#..............#            #                                                      #",
    "#..............#            #                                                      #",
    "#..............#            #                                                      #",
    "#..............#            #                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............#                                                      #",
    "#..............#............################                     #                 #",
    "#...........................#....................................#                 #",
    "#...........................#....................................#                 #",
    "#...........................#....................................#                 #",
    "##################################################################                 #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "#                                                                                  #",
    "####################################################################################",
};






template<int N, int M, class P_Type, class V_Type, class Flow_Candidate, class ...Args>
std::shared_ptr<FluidParent> getFlowType(const std::string &flow_type, char** _field) {
    if constexpr (sizeof...(Args) == 0) {
        throw std::runtime_error("Invalid type Flow");
    }
    else if(getName<Flow_Candidate>() == flow_type) {
        return std::make_shared<FluidCalc<N, M, P_Type, V_Type, Flow_Candidate, false>>(_field);
    }
    else {
        return getFlowType<N, M, P_Type, V_Type, Args...>(flow_type, _field);
    }
}

template<int N, int M, class P_Type, class V_Candidate, class ...Args>
std::shared_ptr<FluidParent> getVType(const std::string &v_type, const std::string &flow_type, char** _field) {
    if constexpr (sizeof...(Args) == 0) {
        throw std::runtime_error("Invalid type V");
    }
    else if (getName<V_Candidate>() == v_type) {
        return getFlowType<N, M, P_Type, V_Candidate, Args...>(flow_type, _field);
    }
    else{
        return getVType<N, M, P_Type, Args...>(v_type, flow_type, _field);
    }
}

template<int N, int M, class P_Candidate, class ...Args>
std::shared_ptr<FluidParent> getPType(const std::string &p_type, const std::string &v_type, const std::string &flow_type, char** _field) {
    if constexpr (sizeof...(Args) == 0) {
        throw std::runtime_error("Invalid type P");
    }
    else if(getName<P_Candidate>() == p_type) {
        return getVType<N, M, P_Candidate, Args...>(v_type, flow_type, _field);
    }
    else{
        return getPType<N, M, Args...>(p_type, v_type, flow_type, _field);
    }
}

#define FLOAT float
#define FAST_FIXED(width, precision) Fixed<FastInt, width, precision>
#define FIXED(width, precision) Fixed<Int, width, precision>
#define DOUBLE double

// 3 раза вставляем TYPES т.к. каждый раз нужно пройтись по всем типам
#define S(a, b) {{a, b}, {getPType<a, b, TYPES, TYPES, TYPES>(params["p-type"], params["v-type"], params["flow-type"], actual_field)}}

#ifndef SIZES
#define SIZES S(1920,1080),S(10,10),S(42,1337)
#endif

#ifndef TYPES
#define TYPES FAST_FIXED(54,16),FIXED(32,16),DOUBLE,FLOAT
#endif

int main(int argc, char* argv[]) {
    std::map<std::string, std::string> params;
    std::regex pattern("--(p-type|v-type|flow-type|file)=(.+)");

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        std::smatch match;
        if (std::regex_match(arg, match, pattern)) {
            params[match[1]] = match[2];
        }
    }

    if (params.find("file") == params.end()) {
        throw std::runtime_error("Missing mandatory argument: --file");
    }

    std::cout << "Parsed parameters:\n";
    for (const auto& [key, value] : params) {
        std::cout << key << ": " << value << std::endl;
    }

    int actualN, actualM;
    std::ifstream file(params["file"]);
    file >> actualN >> actualM;
    char **actual_field = new char*[actualN];
    std::map<std::pair<int, int>, std::shared_ptr<FluidParent>> allFluids = {SIZES};

    for (int i = 0; i < actualN; ++i) {
        actual_field[i] = new char[actualM + 1];
        std::string line;
        std::getline(file, line);
        std::strncpy(actual_field[i], line.c_str(), actualM);
        actual_field[i][actualM] = '\0'; // Ensure null-termination
    }

    if(!allFluids.contains({actualN, actualM})) {
        std::cout << "Such size does not exist\n";
        std::cout << "Fallback to dynamic Fluid...\n";
        //create dynamic fluid
        //auto fluid = getPType<0, 0, TYPES, TYPES, TYPES>(params["p-type"], params["v-type"], params["flow-type"], actual_field);
    }
    else {
        auto fluid = allFluids[{actualN, actualM}];
        fluid->run(1000);
    }


    return 0;
}