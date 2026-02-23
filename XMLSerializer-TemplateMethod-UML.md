# Template Method Pattern — XMLSerializer UML

```mermaid
classDiagram
    class XMLSerializer {
        <<abstract>>
        +serialize(node)
        +case_document(node)*
        +case_element(node)*
        +case_text(node)*
    }
    class PrettyXMLSerializer {
        <<concrete>>
        +case_document(node)
        +case_element(node)
        +case_text(node)
    }
    class MinimalXMLSerializer {
        <<concrete>>
        +case_document(node)
        +case_element(node)
        +case_text(node)
    }
    XMLSerializer <|-- PrettyXMLSerializer
    XMLSerializer <|-- MinimalXMLSerializer
```

## Legend

| Role | Class / Method |
|------|----------------|
| **Abstract class** | `XMLSerializer` |
| **Concrete classes** | `PrettyXMLSerializer`, `MinimalXMLSerializer` |
| **Template method** | `serialize(dom::Node *node)` — defines the algorithm and calls the primitives |
| **Primitive methods** | `case_document`, `case_element`, `case_text` — abstract in base, implemented in concrete classes |
