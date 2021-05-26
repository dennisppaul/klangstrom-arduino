---
layout: libdoc
title: Klang ( Sound + Digital Audio Signal Processing )
permalink: /klang/
index: 20
---

*Klang* is the sound and *digital audio signal processing* (DSP) library of *Klangstrom* or put in other words: *Klang* is a node+text-based sound synthesis library.

although *Klang* is *text-based* it uses a *node-based model*. *nodes* are objects with inputs and outputs that are connected to form signal networks called *patches*. *Klang* comes with a set of *nodes* ( see below ) but can also be extended with custom nodes.

see [Working with Nodes in Klang]({{ site.baseurl }}{% link _manual/working-with-nodes-in-klang.md %}) to learn more about nodes.

## Available Nodes

<ul>
{% assign items = site.klang | sort: 'index' %}
{% for page in items %}
<li><code><a href="{{ page.url | relative_url }}">{{ page.title }}</a></code></li>
{% endfor %}
</ul>