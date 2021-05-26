---
layout: libdoc
title: setup
permalink: /klangstrom/setup/
index: 100
tag: implement
---

called once at application start.

```c
void setup();
```

- it is used to configure the application. peripherals and node patches are usually configured here. ( examples )
- some thing happen right before and after this is called

see [`option`]({{ site.baseurl }}{% link _klangstrom/option.md %})