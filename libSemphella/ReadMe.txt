<<<<<<< HEAD:win32/libSemphella/ReadMe.txt
﻿========================================================================
    动态链接库：libSemphella 项目概述
========================================================================

应用程序向导已为您创建了此 libSemphella DLL。

本文件概要介绍组成 libSemphella 应用程序的每个文件的内容。


libSemphella.vcxproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，其中包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

libSemphella.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。它包含有关项目文件与筛选器之间的关联信息。在 IDE 中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。例如，“.cpp”文件与“源文件”筛选器关联。

libSemphella.cpp
    这是主 DLL 源文件。

	此 DLL 在创建时不导出任何符号。因此，生成时不会产生 .lib 文件。如果希望此项目成为其他某个项目的项目依赖项，则需要添加代码以从 DLL 导出某些符号，以便产生一个导出库，或者，也可以在项目“属性页”对话框中的“链接器”文件夹中，将“常规”属性页上的“忽略输入库”属性设置为“是”。

/////////////////////////////////////////////////////////////////////////////
其他标准文件:

StdAfx.h, StdAfx.cpp
    这些文件用于生成名为 libSemphella.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

/////////////////////////////////////////////////////////////////////////////
其他注释:

应用程序向导使用“TODO:”注释来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////
=======
========================================================================
    控制台应用程序：runner 项目概述
========================================================================

应用程序向导已为您创建了此 runner 应用程序。

本文件概要介绍组成 runner 应用程序的每个文件的内容。


runner.vcxproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，其中包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

runner.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。它包含有关项目文件与筛选器之间的关联信息。在 IDE 中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。例如，“.cpp”文件与“源文件”筛选器关联。

runner.cpp
    这是主应用程序源文件。

/////////////////////////////////////////////////////////////////////////////
其他标准文件:

StdAfx.h, StdAfx.cpp
    这些文件用于生成名为 runner.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

/////////////////////////////////////////////////////////////////////////////
其他注释:

应用程序向导使用“TODO:”注释来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////
>>>>>>> 63eb7ab1163f5082c8c315a32d73c8e0bdaf29b2:tools/win32/PT/ReadMe.txt
