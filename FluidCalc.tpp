#pragma once
#include <bits/stdc++.h>

template <class T>
auto getName() {
    if constexpr (std::is_same_v<T, float>) {
        return "FLOAT";
    } else if constexpr (std::is_same_v<T, double>) {
        return "DOUBLE";
    } else if constexpr (std::is_class_v<T> && std::is_invocable_v<decltype(&T::get_name)>) {
        return T::get_name();
    } else {
        return "getName called from UNKNOWN TYPE";
    }
}

static constexpr std::array<std::pair<int, int>, 4> deltas{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
using std::pair, std::tuple, std::array, std::swap, std::cout, std::min;

namespace std {
    template<typename T, std::size_t N>
    void swap(std::array<T, N>& a, std::array<T, N>& b) {
        for (std::size_t i = 0; i < N; ++i) {
            std::swap(a[i], b[i]);
        }
    }
    template<typename T, typename U, std::size_t N>
    void swap(std::array<T, N>& a, std::array<U, N>& b) {
        for (std::size_t i = 0; i < N; ++i) {
            std::swap(static_cast<T>(a[i]), static_cast<T>(b[i]));
        }
    }
}
class FluidParent {
public:
    FluidParent() = default;
    ~FluidParent() = default;
    virtual void run(size_t) = 0;
};

template <int N, int M, class p_Type, class v_Type, class flow_Type, bool dynamic>
struct FluidCalc : public FluidParent{
    // If not dynamic, must create static field N * M
    using fieldType = std::conditional_t<!dynamic, std::array<std::array<char, M+1>, N>, char(*)[M+1]>;
    fieldType field;

    inline static int MyN = N;
    inline static int MyM = M;

    //For static implementation
    template <bool D = dynamic, std::enable_if_t<!D, int> = 0>
    explicit FluidCalc(char** _field): FluidParent() {

        std::cout << "I am constructed with Types: "
                  << getName<p_Type>() << ", "
                  << getName<v_Type>() << ", "
                  << getName<flow_Type>()
                  << std::endl;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j <= M; ++j) {
                field[i][j] = _field[i][j];
            }
        }
    }


    //For dynamic implementation
    template <bool D = dynamic, std::enable_if_t<D, int> = 0>
    explicit FluidCalc(char** _field, int N_, int M_) : FluidParent() {
        field = _field;
        MyN = N_;
        MyM = M_;
    }

    inline static p_Type inf = p_Type::max();
    inline static p_Type eps = p_Type::from_raw(deltas.size());


    inline static p_Type rho[256];

    inline static p_Type p[N][M]{}, old_p[N][M];

    template<typename T>
    struct VectorField {
        VectorField() {
            ;
        }
        array<T, deltas.size()> v[N][M];

        T& add(int x, int y, int dx, int dy, T dv) {
            return get(x, y, dx, dy) += dv;
        }

        T& get(int x, int y, int dx, int dy) {
            size_t i = std::ranges::find(deltas, pair(dx, dy)) - deltas.begin();
            assert(i < deltas.size());
            return v[x][y][i];
        }
        public:
        VectorField& operator=(VectorField&& other) noexcept {
            if (this != &other) {
                for (size_t i = 0; i < N; ++i) {
                    for (size_t j = 0; j < M; ++j) {
                        std::copy(std::begin(other.v[i][j]), std::end(other.v[i][j]), std::begin(v[i][j]));
                    }
                }
            }
            return *this;
        }
    };

    inline static VectorField<v_Type> velocity{};
    inline static VectorField<flow_Type> velocity_flow{};
    inline static int last_use[N][M]{};
    inline static int UT = 0;


    inline static std::mt19937 rnd{1337};

    tuple<p_Type, bool, pair<int, int>> propagate_flow(int x, int y, p_Type lim) {
        last_use[x][y] = UT - 1;
        p_Type ret = 0;
        for (auto [dx, dy] : deltas) {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT) {
                auto cap = velocity.get(x, y, dx, dy);
                auto flow = velocity_flow.get(x, y, dx, dy);
                if (static_cast<double>(flow) == static_cast<double>(cap)) {
                    continue;
                }
                // assert(v >= velocity_flow.get(x, y, dx, dy));
                auto vp = min(static_cast<double>(lim), static_cast<double>(cap - flow));
                if (last_use[nx][ny] == UT - 1) {
                    velocity_flow.add(x, y, dx, dy, vp);
                    last_use[x][y] = UT;
                    // cerr << x << " " << y << " -> " << nx << " " << ny << " " << vp << " / " << lim << "\n";
                    return {vp, 1, {nx, ny}};
                }
                auto [t, prop, end] = propagate_flow(nx, ny, vp);
                ret += t;
                if (prop) {
                    velocity_flow.add(x, y, dx, dy, t);
                    last_use[x][y] = UT;
                    // cerr << x << " " << y << " -> " << nx << " " << ny << " " << t << " / " << lim << "\n";
                    return {t, prop && end != pair(x, y), end};
                }
            }
        }
        last_use[x][y] = UT;
        return {ret, 0, {0, 0}};
    }

    static p_Type random01() {
        if constexpr (std::is_same_v<p_Type, double>) {
            return static_cast<p_Type>(rnd()) / rnd.max();
        }
        else if constexpr (std::is_same_v<p_Type, float>) {
            return static_cast<p_Type>(rnd()) / rnd.max();
        }
        else {
            return p_Type::from_raw(rnd());
        }
    }

    void propagate_stop(int x, int y, bool force = false) {
        if (!force) {
            bool stop = true;
            for (auto [dx, dy] : deltas) {
                int nx = x + dx, ny = y + dy;
                if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) > 0) {
                    stop = false;
                    break;
                }
            }
            if (!stop) {
                return;
            }
        }
        last_use[x][y] = UT;
        for (auto [dx, dy] : deltas) {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT || velocity.get(x, y, dx, dy) > 0) {
                continue;
            }
            propagate_stop(nx, ny);
        }
    }

    p_Type move_prob(int x, int y) {
        p_Type sum = 0;
        for (size_t i = 0; i < deltas.size(); ++i) {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
                continue;
            }
            auto v = velocity.get(x, y, dx, dy);
            if (v < 0) {
                continue;
            }
            sum += v;
        }
        return sum;
    }

    struct ParticleParams {
        char type;
        p_Type cur_p;
        array<p_Type, deltas.size()> v;
        fieldType &field;
        ParticleParams(fieldType &field) : field(field) {};
        void swap_with(int x, int y) {
            swap(field[x][y], type);
            swap(p[x][y], cur_p);
            //swap(static_cast<decltype(v)>(velocity.v[x][y]), v);
            for(size_t i = 0; i <v.size(); i++) {
                swap(static_cast<decltype(v[i])>(velocity.v[x][y][i]), v[i]);
            }
        }
    };

    bool propagate_move(int x, int y, bool is_first) {
        last_use[x][y] = UT - is_first;
        bool ret = false;
        int nx = -1, ny = -1;
        do {
            std::array<p_Type, deltas.size()> tres;
            p_Type sum = 0;
            for (size_t i = 0; i < deltas.size(); ++i) {
                auto [dx, dy] = deltas[i];
                int nx = x + dx, ny = y + dy;
                if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
                    tres[i] = sum;
                    continue;
                }
                auto v = velocity.get(x, y, dx, dy);
                if (v < 0) {
                    tres[i] = sum;
                    continue;
                }
                sum += v;
                tres[i] = sum;
            }

            if (sum == 0) {
                break;
            }

            p_Type p = random01() * sum;
            size_t d = std::ranges::upper_bound(tres, p) - tres.begin();

            auto [dx, dy] = deltas[d];
            nx = x + dx;
            ny = y + dy;

            assert(field[nx][ny] != '#');
            assert(last_use[nx][ny] < UT);
            assert(velocity.get(x, y, dx, dy) > 0);

            ret = (last_use[nx][ny] == UT - 1 || propagate_move(nx, ny, false));
        }
        while (!ret);
        last_use[x][y] = UT;
        for (size_t i = 0; i < deltas.size(); ++i) {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) < 0) {
                propagate_stop(nx, ny);
            }
        }
        if (ret) {
            if (!is_first) {
                ParticleParams pp{field};
                pp.swap_with(x, y);
                pp.swap_with(nx, ny);
                pp.swap_with(x, y);
            }
        }
        return ret;
    }

    void run(size_t tics) override {
        int dirs[N][M]{};
        rho[' '] = 0.01;
        rho['.'] = 1000;
        p_Type g = 0.1;

        for (size_t x = 0; x < N; ++x) {
            for (size_t y = 0; y < M; ++y) {
                if (field[x][y] == '#')
                    continue;
                for (auto [dx, dy] : deltas) {
                    dirs[x][y] += (field[x + dx][y + dy] != '#');
                }
            }
        }

        for (size_t i = 0; i < tics; ++i) {
            p_Type total_delta_p = 0;
            // Apply external forces
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] == '#')
                        continue;
                    if (field[x + 1][y] != '#')
                        velocity.add(x, y, 1, 0, g);
                }
            }

            // Apply forces from p
            memcpy(old_p, p, sizeof(p));
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] == '#')
                        continue;
                    for (auto [dx, dy] : deltas) {
                        int nx = x + dx, ny = y + dy;
                        if (field[nx][ny] != '#' && old_p[nx][ny] < old_p[x][y]) {
                            auto delta_p = old_p[x][y] - old_p[nx][ny];
                            auto force = delta_p;
                            auto& contr = velocity.get(nx, ny, -dx, -dy);
                            if (contr * rho[(int)field[nx][ny]] >= force) {
                                contr -= force / rho[(int)field[nx][ny]];
                                continue;
                            }
                            force -= contr * rho[(int)field[nx][ny]];
                            contr = 0;
                            velocity.add(x, y, dx, dy, force / rho[(int)field[x][y]]);
                            p[x][y] -= force / dirs[x][y];
                            total_delta_p -= force / dirs[x][y];
                        }
                    }
                }
            }

            // Make flow from velocities
            velocity_flow = {};
            bool prop = false;
            do {
                UT += 2;
                prop = 0;
                for (size_t x = 0; x < N; ++x) {
                    for (size_t y = 0; y < M; ++y) {
                        if (field[x][y] != '#' && last_use[x][y] != UT) {
                            auto [t, local_prop, _] = propagate_flow(x, y, 1);
                            if (t > 0) {
                                prop = 1;
                            }
                        }
                    }
                }
            }
            while (prop);

            // Recalculate p with kinetic energy
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] == '#')
                        continue;
                    for (auto [dx, dy] : deltas) {
                        v_Type old_v = velocity.get(x, y, dx, dy);
                        flow_Type new_v = velocity_flow.get(x, y, dx, dy);
                        if (old_v > 0) {
                            assert(static_cast<double>(new_v) <= static_cast<double>(old_v));
                            velocity.get(x, y, dx, dy) = static_cast<double>(new_v);
                            auto force = (static_cast<double>(old_v) - static_cast<double>(new_v)) * static_cast<double>(rho[(int)field[x][y]]);
                            if (field[x][y] == '.')
                                force *= 0.8;
                            if (field[x + dx][y + dy] == '#') {
                                p[x][y] += force / dirs[x][y];
                                total_delta_p += force / dirs[x][y];
                            }
                            else {
                                p[x + dx][y + dy] += force / dirs[x + dx][y + dy];
                                total_delta_p += force / dirs[x + dx][y + dy];
                            }
                        }
                    }
                }
            }

            UT += 2;
            prop = false;
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] != '#' && last_use[x][y] != UT) {
                        if (random01() < move_prob(x, y)) {
                            prop = true;
                            propagate_move(x, y, true);
                        }
                        else {
                            propagate_stop(x, y, true);
                        }
                    }
                }
            }

            if (prop) {
                system("clear");
                cout << "Tick " << i << ":\n";
                for (size_t x = 0; x < N; ++x) {
                    for (size_t y = 0; y < M; ++y) {
                        cout << static_cast<double>(field[x][y]);
                    }
                    cout << "\n";
                }
            }
        }
    }
};
