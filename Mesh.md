# Mesh
## 先通过构造函数Mesh mesh,获得VAO,VBO,EBO
## 再绑定上传数据到cpu,Mesh::BindBuffer() 并且在内部调用Mesh::Location_Ptr()_获得读取方式
## 解绑VAO,VAO是设计出来方便复用的，就像枪里面的弹匣，私有变量indexcount就是存子弹数的
## 当我们把子弹个数（索引）数清楚后装进弹匣，就可以把之前的值(vertex,index)释放了，因为已经存进VAO了
## 然后调用Mesh::draw() 这个函数就是绑定VAO，化图，解绑