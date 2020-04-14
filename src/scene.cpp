#include "scene.hpp"

// add one element to the list of element in the scene
// Has to derived from Element
void Scene::add_element(Element *newElement)
{
    m_elements.push_back(newElement);
}

// add one light to the list of light
void Scene::add_light(Light newLight)
{
    lightCollection.push_back(newLight);
}

// setter for size of screen
void Scene::set_dimension(int width, int height)
{
    m_width = width;
    m_height = height;
}

// send a ray in virtual world and get its color result
void Scene::send_Ray(Ray ray, int nb, int result[])
{
    bool flag = false;
    float i = 100000, n; // if an object is too far, it will not be seen
    Element *frontElement;
    for (Element *object : m_elements)
    {
        if (object->intersect(ray, n))
        {
            if (n < i)
            {

                i = n;
                flag = true;
                frontElement = object;
            }
        }
    }

    if (flag)
    {
        float point[3];
        ray.getIntersectionPoint(i, point);

        Ray normal = frontElement->getNormal(point);
        if (frontElement->getTransparency() == 0) // if the element will only diffuse
        {
            float contribution; // will store the light contribution to the color on the point
            if (lightCollection.empty())
            {
                contribution = normal.dotLight(ray); // diffusion model of phong from the camera ray
                result[0] = frontElement->getColors()[0] * contribution;
                result[1] = frontElement->getColors()[1] * contribution;
                result[2] = frontElement->getColors()[2] * contribution;
                return;
            }
            else // if there are light in the scene
            {
                // should iterate on light but here consider only one light

                // get light contribution
                Ray lightRay = Ray(point, lightCollection[0].getPoint());
                float specularContribution = pow(max((float)0.0, lightRay.dotLight(normal)), 300);
                contribution = normal.dotLight(lightRay);
                float lightcontribution = (contribution + 1) / 2;
                // loop to check if there is an object between the light and the object
                for (Element *object : m_elements)
                {
                    if (object->intersect(lightRay, n) && object != frontElement && lightRay.m_norm > n)
                    {
                        // if there is an object

                        //check if object is not transparent and nb of reflexion not to much
                        if (object->getTransparency() == 0 || nb > 3)
                        {
                            // 0.5 is abritary
                            result[0] = frontElement->getColors()[0] * 0.5 * lightcontribution;
                            result[1] = frontElement->getColors()[1] * 0.5 * lightcontribution;
                            result[2] = frontElement->getColors()[2] * 0.5 * lightcontribution;
                            return;
                        }
                        else
                        {
                            // if it is trasparente, send a new ray
                            int transparency_contribution[3];
                            send_Ray(lightRay, nb + 1, transparency_contribution);
                            result[0] = 0.8 * frontElement->getColors()[0] * 0.3 * lightcontribution + 0.2 * transparency_contribution[0];
                            result[1] = 0.8 * frontElement->getColors()[1] * 0.3 * lightcontribution + 0.2 * transparency_contribution[1];
                            result[2] = 0.8 * frontElement->getColors()[2] * 0.3 * lightcontribution + 0.2 * transparency_contribution[2];
                            return;
                        }
                    }
                }
                // if the element has no texture
                if (frontElement->getHasTexture() == 0)
                {
                    // add diffusion
                    result[0] = frontElement->getColors()[0] * lightcontribution;
                    result[1] = frontElement->getColors()[1] * lightcontribution;
                    result[2] = frontElement->getColors()[2] * lightcontribution;
                    // add specular on the phong model
                    result[0] = (1 - specularContribution) * result[0] + specularContribution * 255;
                    result[1] = (1 - specularContribution) * result[1] + specularContribution * 255;
                    result[2] = (1 - specularContribution) * result[2] + specularContribution * 255;
                    return;
                }
                else
                {
                    // if element has a texture
                    float u, v;
                    int color[3];
                    frontElement->getTextureUV(point, u, v, color);
                    result[0] = color[0] * lightcontribution;
                    result[1] = color[1] * lightcontribution;
                    result[2] = color[2] * lightcontribution;
                    // add specular on the phong model
                    result[0] = (1 - specularContribution) * result[0] + specularContribution * 255;
                    result[1] = (1 - specularContribution) * result[1] + specularContribution * 255;
                    result[2] = (1 - specularContribution) * result[2] + specularContribution * 255;

                    return;
                }
                return;
            }
        }
        else // if the element is not only diffusion
        {
            if (nb < 3) // check the number of recursion if not too big
            {
                // reflexion
                Ray reflexion_ray = ray.reflectRay(normal, point);
                int reflexion_color[3];
                send_Ray(reflexion_ray, nb + 1, reflexion_color);

                // refraction
                Ray refraction_ray = ray.refractRay(normal, point, frontElement->getTransparency());
                int refraction_color[3];
                if (refraction_ray.m_norm != 0)
                {
                    send_Ray(refraction_ray, nb + 1, refraction_color);
                }

                // fresnel
                float fr = sqrt(ray.getFresnelCoeff(normal, frontElement->getTransparency()));
                result[0] = (1 - fr) * (double)refraction_color[0] + fr * (double)reflexion_color[0];
                result[1] = (1 - fr) * (double)refraction_color[1] + fr * (double)reflexion_color[1];
                result[2] = (1 - fr) * (double)refraction_color[2] + fr * (double)reflexion_color[2];
                Ray lightRay = Ray(point, lightCollection[0].getPoint());
                // add specular
                float specularContribution = pow(max((float)0.0, lightRay.dotLight(normal)), 300);
                result[0] = (1 - specularContribution) * result[0] + specularContribution * 255;
                result[1] = (1 - specularContribution) * result[1] + specularContribution * 255;
                result[2] = (1 - specularContribution) * result[2] + specularContribution * 255;
                return;
            }
            // return shadow if the recursion is too much (physically, may consider that the light has no more energy)
            result[0] = 50;
            result[2] = 50;
            result[1] = 50;
        }
    }

    // if no object then use background color
    result[0] = bg_color[0];
    result[1] = bg_color[0];
    result[2] = bg_color[0];

    return;
}

// function that send the ray and save the output image
void Scene::render_scene()
{
    float res = m_res;
    std::cout << "Resolution of " << res << std::endl;
    float eye[3] = {m_eye_position[0], m_eye_position[1], m_eye_position[2]};
    //initialisation for ppm rendering
    std::ofstream ofs("output/output.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n"
        << this->m_width * (int)res << " " << this->m_height * (int)res << "\n255\n";
    //end of initialisation
    int i, j;
    // each pixel of the screen

    for (j = 0; j < m_height * res; ++j) //lines
    {
        int list_result[(int)(3 * m_width * res)];
#pragma omp parallel for private(i) shared(j, list_result) num_threads(3) // change num_thread to change number of thread 1 == linear
        for (i = 0; i < (int)(m_width * res); ++i) //column
        {
            float *pixel = new float[3];
            pixel[0] = i / (float)res;
            pixel[1] = 0;
            pixel[2] = j / (float)res;
            Ray primary_ray = Ray(eye, pixel);
            int result[3];
            send_Ray(primary_ray, 0, result);
            list_result[(int)(3 * i)] = (result[0]);
            list_result[(int)(3 * i + 1)] = (result[1]);
            list_result[(int)(3 * i + 2)] = (result[2]);
            // ofs << (unsigned char)(result[0]);
            // ofs << (unsigned char)(result[1]);
            // ofs << (unsigned char)(result[2]);
        }
        for (int k = 0; k < 3 * m_width * res; ++k)
        {
            ofs << (unsigned char)list_result[k];
        }
    }
    ofs.close();
}

Scene::Scene()
{
    bg_color[0] = 0;
    bg_color[1] = 0;
    bg_color[2] = 0;
    m_width = 10;
    m_height = 10;
    m_eye_position[0] = this->m_width / 2.;
    m_eye_position[1] = -600;
    m_eye_position[2] = m_height / 2.;
}

Scene::Scene(int width, int height)
{
    bg_color[0] = 0;
    bg_color[1] = 0;
    bg_color[2] = 0;
    m_width = width;
    m_height = height;

    m_eye_position[0] = this->m_width / 2.;
    m_eye_position[1] = -600;
    m_eye_position[2] = m_height / 2.;
}

Scene::Scene(int width, int height, float eye_position, float res)
{
    bg_color[0] = 0;
    bg_color[1] = 0;
    bg_color[2] = 0;
    m_width = width;
    m_height = height;
    m_res = res;
    m_eye_position[0] = this->m_width / 2.;
    m_eye_position[1] = eye_position;
    m_eye_position[2] = m_height / 2.;
}