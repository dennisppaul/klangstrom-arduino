---
layout: libdoc
title: NodeKernelBlock
permalink: /NodeKernelBlock/
index: 46
---

          [ NODE_KERNEL_BLOCK_      ]       
          +-------------------------+       
          |                         |       
    IN00--| SIGNAL           SIGNAL |--OUT00
          |                         |       
          +-------------------------+       

this node facilitate a simple mechanism to write a *kernel* to process the incoming audio signal per audio block. @note(this node is only usable in text-based programming. the abstract method `kernel` needs to be implemented.)


### Source Code

[NodeKernelBlock.hpp]({{ site.baseurl }}{% link _klang-sources/NodeKernelBlock.hpp.md %})

