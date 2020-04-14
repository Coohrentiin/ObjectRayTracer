#include <cmath>
#include <algorithm>

class Ray
{
public:
    float m_origin[3];
    float m_direction[3];
    float m_normalized[3];
    float m_norm;

    Ray()
    {
        m_norm = 0;
    }

    Ray(const Ray &other) = default;

    Ray(float start[3], float end[3])
    {
        m_origin[0] = start[0];
        m_origin[1] = start[1];
        m_origin[2] = start[2];
        m_direction[0] = end[0] - start[0];
        m_direction[1] = end[1] - start[1];
        m_direction[2] = end[2] - start[2];
        m_norm = sqrt(dot(*this));
        if (m_norm != 0)
        {
            m_normalized[0] = m_direction[0] / m_norm;
            m_normalized[1] = m_direction[1] / m_norm;
            m_normalized[2] = m_direction[2] / m_norm;
        }
        else
        {
            // std::cout << "Problem Initializing Ray" << std::endl;
        }
    }

    float *getIntersectionPoint(float lineParam, float interPoint[3])
    {
        interPoint[0] = m_origin[0] + lineParam * m_normalized[0];
        interPoint[1] = m_origin[1] + lineParam * m_normalized[1];
        interPoint[2] = m_origin[2] + lineParam * m_normalized[2];
        return interPoint;
    }

    float dot(Ray ray)
    {
        return m_direction[0] * ray.m_direction[0] + m_direction[1] * ray.m_direction[1] + m_direction[2] * ray.m_direction[2];
    }

    // the dot product is made with the normalize direction of the vector
    float dotIntersection(Ray ray)
    {
        return m_normalized[0] * ray.m_direction[0] + m_normalized[1] * ray.m_direction[1] + m_normalized[2] * ray.m_direction[2];
    }

    // the dot product is made with the normalize direction of the vector and normalize direction of other vector
    float dotLight(Ray ray)
    {
        return m_normalized[0] * ray.m_normalized[0] + m_normalized[1] * ray.m_normalized[1] + m_normalized[2] * ray.m_normalized[2];
    }

    // for debug purposes
    void printRay()
    {
        std::cout << "[" << m_origin[0] << "," << m_origin[1] << "," << m_origin[2] << "]"
                  << "-->"
                  << "["
                  << m_direction[0] << "," << m_direction[1] << "," << m_direction[2] << "]"
                  << "[" << m_normalized[0] << "," << m_normalized[1] << "," << m_normalized[2] << "]" << std::endl;
    }

    Ray crossProduct(Ray normal)
    # 
    {
        float new_direction[3];
        new_direction[0] = normal.m_direction[1] * m_direction[2] - normal.m_direction[2] * m_direction[1];
        new_direction[1] = normal.m_direction[2] * m_direction[0] - normal.m_direction[0] * m_direction[2];
        new_direction[2] = normal.m_direction[0] * m_direction[1] - normal.m_direction[1] * m_direction[0];
        float new_point[3];
        new_point[0] = m_origin[0] + new_direction[0];
        new_point[1] = m_origin[1] + new_direction[1];
        new_point[2] = m_origin[2] + new_direction[2];
        Ray new_ray = Ray(m_origin, new_point);
        return new_ray;
    }

    Ray reflectRay(Ray normal, float *position)
    {
        float a = 2 * this->dot(normal);
        float x = position[0] * (this->m_normalized[0] - a * normal.m_normalized[0]);
        float y = position[1] * (this->m_normalized[1] - a * normal.m_normalized[1]);
        float z = position[2] * (this->m_normalized[2] - a * normal.m_normalized[2]);
        float *result = new float[3];
        result[0] = x;
        result[1] = y;
        result[2] = z;
        Ray new_ray = Ray(position, result);
        return new_ray;
    }

    Ray refractRay(Ray normal, float *position, float n1)
    {
        float eta = 2.0 - n1;
        float cosi = normal.dot(*this);
        float new_direction[3];
        new_direction[0] = (this->m_normalized[0] * eta - normal.m_direction[0] * (-cosi + eta * cosi));
        new_direction[1] = (this->m_normalized[1] * eta - normal.m_direction[1] * (-cosi + eta * cosi));
        new_direction[2] = (this->m_normalized[2] * eta - normal.m_direction[2] * (-cosi + eta * cosi));
        float new_position[3];
        new_position[0] = position[0] + new_direction[0];
        new_position[1] = position[1] + new_direction[1];
        new_position[2] = position[2] + new_direction[2];
        Ray new_ray = Ray(position, new_position);
        return new_ray;
    }

    float getFresnelCoeff(Ray normal, float n1)
    {
        float fr = 0;
        float cosi = normal.dotLight(*this);
        float etai = 1, etat = n1;
        if (cosi > 0)
        {
            std::swap(etai, etat);
        }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        if (sint >= 1)
        {
            fr = 1;
        }
        else
        {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            fr = (Rs * Rs + Rp * Rp) / 2;
        }
        return fr;
    }
};
