#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
	Texture() {}
	Texture(unsigned char *colors, int xsize, int ysize, int id = 1){
        m_xsize = xsize;
        m_ysize = ysize;
		m_colors = colors;
		m_id = id;
		// for (int i = 0; i < xsize*ysize*3; i++)
		// {
		// 	std::cout << int(m_colors[i]) << std::endl;
		// }
		
		
    }

	unsigned char *m_colors;
	int m_xsize, m_ysize, m_id;

    void getValueUV(float u, float v, int color[3]) const {
	int i = u*m_xsize;
	int j = v*m_ysize;

	if (i < 0) i = 0;
	if (j < 0) j = 0;

	if (i > m_xsize - 1) i = m_xsize - 1;
	if (j > m_ysize - 1) j = m_ysize - 1;

	// std::cout << "i: " << i << " j: " << j << std::endl;

	color[0] = int(m_colors[3 * i + 3 * m_xsize*j]) ;
	color[1] = int(m_colors[3 * i + 3 * m_xsize*j+1]) ;
	color[2] = int(m_colors[3 * i + 3 * m_xsize*j+2]) ;

	return;
}
};
#endif