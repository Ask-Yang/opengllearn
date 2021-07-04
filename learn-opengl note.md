### 1、shader单元模型



### 2、从C++结构体到shader资源

1. 输入C++数据---->CPU buffer（实际上就是数组，只不过是在显存中）
2. 将GPU buffer绑定到shader的槽上
3. 在shader中定义对应的结构体
4. 告诉GPU如何解释shader中输入的数据，以使其对应到shader 中定义的结构体，这一步通过vertexLayout(DX11)或者VertexAttribPointer(OpenGL)来说明，而且一般只有vertex数据需要特别说明。其他的index, constant, texture都只要适当的说明就可以了。

### 3、OpenGL的资源格式

OpenGL的资源一般是给每个资源冠以某个id，以此将其与实体对象地址抽象出来。比如VAO，VBO，EBO这些，只需要指定对应的参数即可。更像是configureable的，而不是programable的。

### 4、公匙和私匙

公匙和私匙是一个相对的概念，而且公匙和私匙并不是用来进行双向传递的。一般是接收方，生产一对公匙和私匙，并将公匙发给发送方，这样发送方发送的数据即使被截获，公匙也泄露了也不能知道信件的内容。当然，如果这个时候，接受方发送数据给发送方，信件和公匙都被截获了自然能知道信件的内容。所以公匙和私匙只能用做单向的，这样就不会被破解。只有用私匙才能破解公匙加密的信件，也就是说用公匙破解不了。传统的对称加密，别人要是知道了钥匙就能破解，但是你又不能不给别人钥匙，要不然别人怎么给你发消息呢，所以钥匙的保存很困难。但是实际上，非对称加密本质上就和对称加密不同，所以最好不要尝试用对称加密的思路去理解。现在就是只要你不把私匙传出去就永远破解不了，但是别人却能够安全的给你发信息。

### 5、纹理绑定

先绑定两个纹理到对应的纹理单元，然后定义哪个uniform采样器对应哪个纹理单元

我们还要通过使用glUniform1i设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元。我们只需要设置一次即可，所以这个会放在渲染循环的前面：

其中，value是指texture units, GL_TEXTURE0, GL_TEXTURE1这些，但是不是GL_TEXTURE0的值，而是0，1，2（分别对应GL_TEXTURE0, GL_TEXTURE1）这些值，这里是真的离谱。而且也不是glGenTextures(1, &textureID)的这个textureID。

```c++
   glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), value);
```

也就是说绑定texture有这样一些步骤：

1. 将texture以字节流读入，然后绑定到GPU上，这时还只是资源，得到对应textureID

2. 在shader中创建texture的变量，形如 uniform sampler2D texture1. 

3. 将shader中的这个uniform变量使用glUniform1i绑定到shader的texture槽中，也就是对应的texture unit，取值参考其上。

4. 将textureID也就是这个GPU资源与对应的texture unit绑定。

也就是

```c++
 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D, texture1);
```

另外，对于shader中的任意的sampler2D变量，如果你没有显示的调用glUniform1i为该变量绑定槽的话，那么OpenGL默认是绑定第一个槽，就是GL_TEXTURE0。无论你有几个sampler2D，它都会绑定到这个上面去。（PS：假如要追求极致的速度，那么可以把几张纹理合并成一张纹理，然后对uv做偏移进行采样，就可以达到采样多张纹理的效果）

### 6、OpenGL状态机

OpenGL状态机讲实话是一个很迷惑的东西，初学者恐怕难以理解OpenGL的各种bind操作。经过几个小时的debug和各种测试，OpenGL的状态机有以下几个特点。

1. OpenGL的bind操作对于不同的GPU对象，一般来说是不会互相影响的。比如说glUseProgram和glBIndTexture就是不会互相影响的。glUseProgram之后对于Shader所做的任何工作，对于Texture都是独立的。也就是说你可以为Uniform sampler2D绑定一个不存在的槽位。
2. OpenGL对于GPU对象的任何操作，都要在bind之后进行。假如你要改变纹理的WrapMode，那么你就要先glBindTexture才行。假如你之前绑定了别的纹理，那么现在这个setWrapMode就是对于别的那个纹理进行的。因为不知道别人（别的程序）有没有在你的这个函数调用前bind过，所以最好所有操作前先bind一次。这或许也是为什么每次执行渲染都需要bind一次吧。



### 7、纹理格式RGB、RGBA引发的错误

如题，类似有程序突然崩溃，debug显示栈空（栈空就是数据没有对应上，原数据太少），出现重影（原数据太多）。
