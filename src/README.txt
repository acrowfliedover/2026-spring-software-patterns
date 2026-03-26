I chose the adapter pattern because it's another name for wrapper. Essentially what we want to accomplish is to use the Xerces interface over our current DOM implementation, so that an Xerces user can use our project. 
It is not the bridge pattern because we don't have any refined abstractions. We don't have another layer of methods we want to create, we just want to translate the existing methods of one interface to another. 

We are adding a XercesConvert.hpp file to convert XMLCh strings to standard strings.
We add some wrapper to the Element, Text and Attr so they can be fully utilized. 
We are using adaptation by composition since we don't want to modify the target or adaptee's code. 