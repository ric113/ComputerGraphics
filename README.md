# NCTU-Course  ComputerGraphics
##Homework 01 : PPAP
* Animation Mode (press 'Enter' ):
![](https://github.com/ric113/NCTU-Course---Computer-Graphics/blob/master/OpenGL_HW01/Animation.png)
* Display Mode (press and key except 'Esc' & 'Enter'):
![](https://github.com/ric113/NCTU-Course---Computer-Graphics/blob/master/OpenGL_HW01/DIsplay.png)
* Exit (press 'Esc')

--

##Homework 02 : Stencil/Depth buffer blending
![](https://github.com/ric113/NCTU-Course---Computer-Graphics/blob/master/OPENGL_HW02/Scene.png)
* You can press 'W','A','S','D' to move camera , press 'Q','E' to turn .

--

##Homework 03 : Shading

參考的OpenGL教程 :
	https://learnopengl-cn.github.io/

	 VAO/VBO :
			https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
			http://blog.csdn.net/xiajun07061225/article/details/7628146
	 Uniform : 
			https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/#uniform

助教提供的Library :
	
	 glm/		 : 和Model相關的一些DataStructure & function
	 shader_lib/  : CreateShader()/CreateProgram()/UseProgram()

使用的 Phong Shading 公式 :
	 
	I = Ka+Texture.rgb(l⋅n)+Ks(v⋅r)^α
		I : 最後的outColor , Ka(float) : ambient 強度 , l : ligntPos - Pos
	 	n : Normal , Ks(float) : Specular 強度 , v : viewPos - Pos
	 	r : 光反射方向 , α(int) : 常數

Flat Shading :
	
	將每個Vertex的Normal都使用該三角形的Normal即可 , 並取消Specular效果(僅用Ambient & Diffuse) .

