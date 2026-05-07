# shader类：
- 先构造shader：
- 声明着色器源码 .glsl,通过readFile()拿到源码
- 声明顶点着色器和片段着色器(vertex__shader,fragment_shader)
- 拿到着色器源码并通过compileshader()编译两个着色器并判断成功与否
- 创建项目id 
- 再将两个着色glAttachShader()附着到项目
- 将着色器链接到项目id并检查 checkLinkFalse()
- 不需要着色器了删除 glDeleteShader()

## 析构函数：
- 虽然析构函数会自动释放内存，但是数据传到cpu不能释放，只能释放gpu,要显示析构~~shader(){glDeleteprogram(shaderProgramID)}

## 给资源着色器源码传参
- 传入texure1,用setInt("texture1",0)原理同下,第二个参数是给零个纹理
- 要传入model,view,project,viewPos,lightPos,lightColor
- 由于gpu的shader程序里的uniform变量，和现代cpu的cpp代码不在一个地址空间需要用glGetUniformLocation找到内部位置，然后用glUniformXXX把数据传进去
- 我把这个过程封装成了 setMat4("model",model)