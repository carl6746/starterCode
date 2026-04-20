class Mirror : public Shader {
public:
    Mirror(const color& albedo = {1.0f, 1.0f, 1.0f}) : albedo(albedo) {}

    bool is_mirror() override{
        return true;
    }
    point3 getLight() override {
        return {0.0f, 0.0f, 0.0f}; // not used for mirrors
    }
    color rayColor(const HitStruct& h, int depth, bool inShadow) override {
        return {0.0f, 0.0f, 0.0f}; // not used, Scene handles reflections
    }
    color getShaderColor() {
        return color({0.0f,0.0f,0.0f});
    }

private:
    color albedo;

    vec3 reflect(const vec3& v, const vec3& n) const {
        return v - 2*dot(v, n)*n;
    }
};