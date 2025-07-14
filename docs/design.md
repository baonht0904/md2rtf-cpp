# Design

## 1. General Objective

- Convert standard Markdown documents into Rich Text Format (RTF)
- Ensure extensibility, maintainability, and easy integration into modern C++ projects

## 2. Requirements

**Functional Requirements**

Support for Basic Markdown Elements

- Heading: H1-H6 (lines starting with #, mapped to RTF with different font sizes)
- Text formatting: Bold, italic, strikethrough
- Lists: Ordered and unordered lists
- Links: Hyperlinks in the form [text](url)
- Code blocks: Code blocks (```) and inline code (`)
- Tables: Tables (lines containing the | character)
- Blockquotes: Blockquotes (lines starting with >)
- Horizontal rules: Horizontal lines (---, \*\*\*, \_\_\_)

**Non-Functional Requirements**

- Performance: Efficiently handle large files, optimized for memory and processing time.
- Cross-platform: Support Windows, Linux, and macOS.
- Minimal external dependencies: Prefer using standard C++ libraries; if external dependencies are used, they must be clear and easy to integrate.
- Maintainability: Clear, modern C++ code, easy to extend and test.

## 3. Overall operation

```
    Markdown --Paser--> Abstract Syntax Tree --Generator--> RTF
```

## 4. Abstract Syntax Tree (AST)

- **Document node**: containing the entire document content as child nodes.

- **Block nodes**: Represent block-level elements like:
  |Node type|Main properties|Notes|
  |--|--|--|
  |Heading|level, children|children = inline nodes|
  |Paragraph|children|children = inline nodes|
  |Blockquote|children|children = inline nodes|
  |List|ordered, children|children = list items|
  |List Item|children|children = block nodes|
  |Table|children|children = table rows|
  |Table Row|isHeader, children|children = table cell nodes|
  |TableCell|children, align(optional)|children = inline nodes|
  |Code Block|value, language (optional)|value = code string|
  |Horizontal Rule|- |- |

- **Inline nodes**: Represent inline elements like:
  |Node type|Main properties|Meaning|
  |--|--|--|
  |Text|value|plain text|
  |Emphasis|children|supports nested formatting|
  |Strong|children|supports nested formatting|
  |Strikethrough|children|supports nested formatting|
  |Inline code|value|E.g. `code`|
  |Link|url, children, title(optional)|supports nested formatting|
  |Image|url, alt, title (optional)|E.g. ![alternative description](url)|

- **Other elements** are treated as paragraph or text nodes.

- **Example**

  - Markdown

    ```markdown
    # Hello World

    This is a **bold** and _italic_ text.

    - Item 1
    - Item 2

    [Link](https://example.com)

    | Name  | Age |
    | ----- | --- |
    | Alice | 30  |
    | Bob   | 25  |
    ```

  - AST (in JSON-like format)

    ```json
    {
      "type": "document",
      "children": [
        {
          "type": "heading",
          "level": 1,
          "children": [{ "type": "text", "value": "Hello World" }]
        },
        {
          "type": "paragraph",
          "children": [
            { "type": "text", "value": "This is a " },
            {
              "type": "strong",
              "children": [{ "type": "text", "value": "bold" }]
            },
            { "type": "text", "value": " and " },
            {
              "type": "emphasis",
              "children": [{ "type": "text", "value": "italic" }]
            },
            { "type": "text", "value": " text." }
          ]
        },
        {
          "type": "list",
          "ordered": false,
          "children": [
            {
              "type": "listItem",
              "children": [
                {
                  "type": "paragraph",
                  "children": [{ "type": "text", "value": "Item 1" }]
                }
              ]
            },
            {
              "type": "listItem",
              "children": [
                {
                  "type": "paragraph",
                  "children": [{ "type": "text", "value": "Item 2" }]
                }
              ]
            }
          ]
        },
        {
          "type": "paragraph",
          "children": [
            {
              "type": "link",
              "url": "https://example.com",
              "title": null,
              "children": [{ "type": "text", "value": "Link" }]
            }
          ]
        },
        {
          "type": "table",
          "children": [
            {
              "type": "tableRow",
              "isHeader": true,
              "children": [
                {
                  "type": "tableCell",
                  "align": null,
                  "children": [{ "type": "text", "value": "Name" }]
                },
                {
                  "type": "tableCell",
                  "align": null,
                  "children": [{ "type": "text", "value": "Age" }]
                }
              ]
            },
            {
              "type": "tableRow",
              "isHeader": false,
              "children": [
                {
                  "type": "tableCell",
                  "align": null,
                  "children": [{ "type": "text", "value": "Alice" }]
                },
                {
                  "type": "tableCell",
                  "align": null,
                  "children": [{ "type": "text", "value": "30" }]
                }
              ]
            },
            {
              "type": "tableRow",
              "isHeader": false,
              "children": [
                {
                  "type": "tableCell",
                  "align": null,
                  "children": [{ "type": "text", "value": "Bob" }]
                },
                {
                  "type": "tableCell",
                  "align": null,
                  "children": [{ "type": "text", "value": "25" }]
                }
              ]
            }
          ]
        }
      ]
    }
    ```

## 5. Mapping Syntax: AST, Markdown, and RTF

### Block Elements

| AST Node      | Markdown Syntax                  | RTF Representation                        |
|---------------|----------------------------------|--------------------------------------------|
| Document      | *(implicit)*                     | `{...}` (RTF group for document)           |
| Heading       | `#`, `##`, ...                   | `\fsN\b` (font size + bold)                |
| Paragraph     | *(blank line separated)*         | `\par` (paragraph break)                   |
| Blockquote    | `> ...`                          | `\liN` (left indent) or custom style       |
| List (ul/ol)  | `-`, `*`, `+` / `1.`, `2.`       | `\pntext`, `\pnlvlblt`, `\pnlvlbody`       |
| List Item     | List marker + content            | List group with `\pntext` and `\par`       |
| Table         | `| col1 \| col2 |`               | `\trowd ... \cell ... \row`                |
| Table Row     | Markdown table row               | RTF row group (`\trowd ... \row`)          |
| Table Cell    | Markdown table cell              | RTF cell (`\cell`)                         |
| Code Block    | Triple backticks or indent       | `\fN` (monospace font), `\cbN` (bg color)  |
| Horizontal Rule | `---`, `***`, `___`            | `\par\qr\brdrb\brdrs\brdrw10\brsp20`       |

### Inline Elements

| AST Node      | Markdown Syntax                  | RTF Representation                        |
|---------------|----------------------------------|--------------------------------------------|
| Text          | Plain text                       | Plain text                                 |
| Emphasis      | `*italic*` or `_italic_`         | `\i ... \i0` (italic on/off)               |
| Strong        | `**bold**` or `__bold__`         | `\b ... \b0` (bold on/off)                 |
| Strikethrough | `~~text~~`                       | `\strike ... \strike0` (strikethrough)     |
| Inline Code   | `` `code` ``                     | `\fN` (monospace font)                     |
| Link          | `[text](url)`                    | `{\field{\*\fldinst HYPERLINK "url"}{\fldrslt text}}` |
| Image         | `![alt](url)`                    | `{\pict ...}` (embedded image)             |
