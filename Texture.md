# Texture
## 内部储存纹理宽高分量值和id(Tw,Th,chTex_id)
## 通过构造函数Texture(const char* path)传入路径
## 再通过stbi_load()存入照片像素，通过format存color分度，知道分度就可以将数据从cpu传入gpu了
## glGenTexture(),glBindTexture()获得绑定纹理器
## glTexImage2D()传入像素数据
## glGenerateMipmap()构建金字塔 
## 处理细节
- glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);S->u,第三个参数是平铺，舍去整数
- glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);T->v
- glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);处理远景，层级插值
- glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);处理近景，单图插值
## 释放cpu的data stbi_image_free()释放因为stbi_load()里面用的malloc
## 绑定纹理器
- glActiveTexture(GL_TEXTURE0+unit) → “我现在要操作 unit 号插槽”
- glBindTexture(GL_TEXTURE_2D, tex__id) → “把这张纹理放到当前活动的插槽里”。