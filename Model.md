Model 类
功能
# 加载并渲染 OBJ 等 3D 模型文件。
## 原理：
- Assimp 把模型文件解析成一个树形结构的场景对象（aiScene），场景里有一个根节点（mRootNode）和很多个网格（aiMesh，你笔记里说的“块”就是这些网格）。根节点下面挂载着子节点和网格索引，通过深度优先遍历这棵树，找到所有网格的索引号，再从场景的网格数组（scene->mMeshes）里取出真正的网格数据。

每个网格的顶点、法线、纹理坐标被提取出来，存入我们自己封装的 Mesh 对象里。渲染时逐网格调用 draw()，屏幕上就出现了完整的 3D 模型。

## 步骤
### 构造函数

1 .Model(const char* path) 调用 loadModel() 加载文件。
同时用 path.substr(0, path.find_last_of('/')) 提取文件路径的文件夹部分，存入 directory（为将来加载与模型同目录的纹理做准备，暂未使用）。

2.loadModel：读取文件
importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals)。

- 三个标志的作用：
aiProcess_Triangulate：把所有非三角形面强制转为三角形。
aiProcess_FlipUVs：翻转纹理的 V 坐标（因为 OpenGL 的纹理坐标系 Y 轴向上，而很多建模软件 Y 轴向下）。
aiProcess_GenNormals：如果模型没有法向量，自动生成。

- 三重检查：if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)：

- !scene：文件根本无法读取（路径错误或格式不支持）。

- mFlags & AI_SCENE_FLAGS_INCOMPLETE：文件部分损坏或不完整。

- !scene->mRootNode：文件加载了但场景树为空。

- 加载成功后，调用 processNode(scene->mRootNode, scene) 从根节点开始遍历。

3.processNode：递归遍历节点树
处理当前节点的所有网格：遍历 node->mMeshes 数组，用里面存的索引号去 scene->mMeshes[...] 找到真正的 aiMesh*，然后调用 processMesh 提取数据。
递归处理所有子节点：遍历 node->mChildren 数组，对每个子节点调用 processNode。

4.processMesh：提取单个网格数据
遍历 mesh->mVertices、mesh->mNormals、mesh->mTextureCoords，按每 8 个 float 一个顶点（位置3 + 法线3 + 纹理2）存入 vector<float>。
遍历 mesh->mFaces，取出所有三角形的顶点索引存入 vector<unsigned int>。
创建 std::unique_ptr<Mesh>，调用 BindBuffer(vertices, indices) 把数据上传到 GPU。
用 meshes.push_back(std::move(m)) 把智能指针移入容器，移交所有权。

- 用智能指针的原因：Mesh 类持有 GPU 资源（VAO/VBO/EBO），禁止拷贝。std::unique_ptr 只允许移动所有权，从语言层面杜绝了“浅拷贝导致 GPU 资源被提前释放”的隐患。

5.Draw()：逐网格渲染
遍历 meshes 中的所有智能指针，对每个 Mesh 调用 draw()，等于对每个子网格调用 glDrawElements，完整模型一次绘制完成。