---
layout: libdoc
title: NODE_KERNEL_BLOCK
permalink: /node_kernel_block/
index: 44
---

          [ NODE_KERNEL_BLOCK      ]       
          +------------------------+       
          |                        |       
    IN00--| SIGNAL          SIGNAL |--OUT00
          |                        |       
          +------------------------+       

this node facilitate a simple mechanism to write a *kernel* to process the incoming audio signal per audio block. @note(this node is only usable in text-based programming. the abstract method `kernel` needs to be implemented.)


