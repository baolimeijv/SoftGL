# SoftGL — OpenGL 渲染器：法线贴图 / 动态点光源 / 多物体

C++ + OpenGL 实现的实时渲染器。GLAD 加载 OpenGL 函数，Assimp 加载模型，
stb_image 加载纹理。

## 技术要点

- **法线贴图**：材质额外绑定切线空间法线贴图（砖墙 / 路面两套），
  片元着色器把法线从切线空间变换到世界空间后参与光照，因此平面能看到凹凸细节
- **动态点光源**：光源位置与颜色作为 uniform 每帧上传，可实时移动观察高光与漫反射变化；
  光照为 Blinn-Phong 模型（环境光 + 漫反射 + 高光 + 距离衰减）
- **多物体同屏**：立方体（法线贴图 + 漫反射贴图）与斯坦福兔子（纯色）共用同一套
  Shader / Mesh / Model 结构；另有一个绿色小立方体实时指示光源位置
- **第一人称相机**：鼠标 yaw / pitch 控制视角，WASD 移动
- **模型加载**：Assimp 解析 OBJ，构建带顶点 / 法线 / UV / 索引的 Mesh
- **纹理**：stb_image 加载 + Mipmap 生成

## 操作

| 按键 | 功能 |
|---|---|
| W / A / S / D | 移动相机 |
| Q / E | 相机下降 / 上升 |
| 鼠标 | 旋转视角 |
| J / L | 光源左右移动 |
| I / K | 光源前后移动 |
| U / P | 光源上下移动 |
| ESC | 退出 |

## 项目结构

```
main.cpp                     # 入口与渲染循环
vertexshadersource.glsl      # 顶点着色器（MVP、切线空间）
fragmentshadersource.glsl    # 片元着色器（法线贴图 + Blinn-Phong 点光源）
Mesh.cpp / Model.cpp         # 网格与模型（Assimp）
shader.cpp / Texture.cpp     # 着色器封装与纹理
TextureSrouce/               # 漫反射贴图 + 法线贴图
bunny_10k.obj                # 斯坦福兔子模型
```

## 依赖

OpenGL、GLFW、GLAD、Assimp、GLM、stb_image（第三方库未入库）
