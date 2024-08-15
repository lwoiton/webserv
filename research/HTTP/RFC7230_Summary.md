# Summary of RFC7230
This RFC is an troduction to the **H**yper**t**est **T**ransform **P**rotocoll (HTTP) in terms of the "Message Syntax and Routing".
## Chapter 1: Introduction
HTTP (Hypertext Transfer Protocol) is a stateless, application-level request/response protocol designed for flexible interaction with network-based hypertext systems. Key characteristics include:

1. Uniform interface: Presents a standardized way for clients to interact with diverse resources and services.
2. Stateless: Each request-response cycle is independent of others.
3. Extensible: Supports new methods, headers, and status codes without breaking existing implementations.
4. Intermediary-friendly: Enables proxies, gateways, and caches for complex network architectures.

The document, part of the HTTP/1.1 specification series, covers:
- Architectural elements and URI schemes ("http" and "https")
- Network operation and connection management
- Message framing and forwarding requirements
It uses ABNF notation for syntax description and defines key terms like "MUST" and "SHOULD" as per RFC2119.

## Chapter 2: Architecture
### 2.1. Client/Server Messaging:

     			request   >
       UA ======================================= O
            			        <   response
- HTTP (Hypertext Transfer Protocol): A stateless request/response protocol for exchanging messages over a reliable connection.
- Client (UA = **U**ser **A**gent): A program that initiates connections to send HTTP requests.
- Server (O = **o**rigin server): A program that accepts connections to service HTTP requests by sending HTTP responses.
- User Agent: Any client program initiating a request (e.g., browsers, spiders, mobile apps).
- Origin Server: The program that can originate authoritative responses for a given target resource.
- Request: A message sent by a client, including method, URI, protocol version, headers, and possibly a body.
- Response: A message sent by a server, including status line, headers, and possibly a body.
- URI (Uniform Resource Identifier): Standard for indicating the target resource and relationships between resources.

### 2.2. Implementation Diversity:

- User Agent Diversity: Includes various devices beyond browsers (e.g., appliances, firmware update scripts).
- Origin Server Diversity: Includes various systems beyond websites (e.g., home automation units, traffic cameras).
- Background User Agents: Agents that run without direct human interaction (e.g., web crawlers).
- Error Reporting: May be limited to error consoles or log files due to implementation diversity.
- User Confirmation: May be handled through configuration choices or run-time options rather than direct interaction.

### 2.3. Intermediaries:

    	       >             >             >             >
       UA =========== A =========== B =========== C =========== O
               <             <             <             <

- Intermediary: A program that acts as both server and client to make requests on behalf of other clients.
- Proxy: A message-forwarding agent selected by the client, often used for security or shared caching.
- Gateway (Reverse Proxy): An intermediary that appears as an origin server for the outbound connection but translates and forwards requests inbound.
- Tunnel: A blind relay between two connections without changing messages, often used for secure communications through firewalls.
- Upstream/Downstream: Directional terms related to message flow (upstream to downstream).
- Inbound/Outbound: Directional terms related to the request route (inbound toward origin server, outbound toward user agent).
- Interception Proxy (Transparent Proxy): An intermediary not selected by the client, often used to enforce network policies.

### 2.4. Caches:

   				s  >             >
          UA =========== A =========== B - - - - - - C - - - - - - O
                  <             <

- Cache: A local store of previous response messages and the system controlling its storage, retrieval, and deletion.
- Cacheable Response: A response that a cache is allowed to store for future use.
- Cache Constraints: Limitations placed by clients or origin servers on when a cached response can be used.
- Cache Chain: The shortened request/response path when a cached response is available.
- Cache Architectures: Various configurations of caches deployed across the web (e.g., national hierarchies, collaborative systems).

### 2.5 Conformance and Error Handling:

- Conformance Criteria: Requirements based on the role of a participant in HTTP communication (sender, recipient, client, server, etc.).
- Generate vs. Send: "Generate" is used when creating a new protocol element, as opposed to merely forwarding one.
- Conformant Implementation: Complies with all requirements associated with its roles in HTTP.
- Syntax and Semantics: Conformance includes both the structure and meaning of protocol elements.
- Parsing Requirements: Recipients must parse any reasonable length value matching the ABNF grammar for their role.
- Reasonable Length: Expectations of appropriate protocol element lengths, which may vary by context and change over time.
- Error Recovery: Recipients may attempt to recover usable elements from invalid constructs, but specific mechanisms are not defined.

### 2.6. Protocol Versioning:

- Version Numbering: HTTP uses a "&lt;major&gt;.&lt;minor&gt;" scheme (e.g., "1.1").
- HTTP-version Field: Indicates the message's protocol version in the first line.
- Major Version: Indicates HTTP messaging syntax.
- Minor Version: Indicates the highest minor version within that major version the sender understands.
- Backwards Compatibility: HTTP/1.1 messages are constructed to be interpretable by HTTP/1.0 recipients.
- Header Field Interpretation: Doesn't change between minor versions of the same major version.
- Version Forwarding: Intermediaries must send their own HTTP-version in forwarded messages.
- Client Version Selection: Should send the highest conformant version not exceeding the server's highest supported version.
- Server Version Selection: Should respond with the highest conformant version not exceeding the request's major version.
- Protocol Downgrade: Servers may send HTTP/1.0 responses if the client is suspected to incorrectly implement later versions.
- Version Incrementing: Major number for incompatible syntax, minor number for additions to semantics or sender capabilities.
- Higher Minor Version Handling: Recipients should process as if it were the highest minor version they conform to within that major version.

### 2.7. Uniform Resource Identifiers (URIs):

- URI: Used throughout HTTP to identify resources.
- URI References: Used for targeting requests, indicating redirects, and defining relationships.
- Key URI Components: Includes scheme, authority, port, host, path, query, and fragment.
- Absolute-path: Defined for protocol elements that can contain a non-empty path component.
- Partial-URI: Defined for protocol elements that can contain a relative URI but not a fragment.

#### 2.7.1. http URI Scheme:

- Purpose: Minting identifiers associated with hierarchical namespace governed by potential HTTP origin servers.
- Structure: "http:" "//" authority path-abempty [ "?" query ] [ "#" fragment ]
- Origin Server: Identified by the authority component (host identifier and optional TCP port).
- Empty Host Identifier: Senders must not generate URIs with empty host identifiers.
- Default Port: TCP port 80 if not specified.
- URI Presence: Does not imply an active HTTP server; anyone can mint a URI.
- Accessing Resources: Clients may attempt access by resolving host, establishing TCP connection, and sending HTTP request.

#### 2.7.2. https URI Scheme:

- Purpose: Similar to http scheme, but for TLS-secured connections.
- Default Port: TCP port 443 if not specified.
- Security Requirement: User agent must ensure a secure connection before sending the first HTTP request.
- Structure: "https:" "//" authority path-abempty [ "?" query ] [ "#" fragment ]
- Distinct Namespace: https resources have no shared identity with http resources, even with same authority.

#### 2.7.3. http and https URI Normalization and Comparison:

- Normalization Process: Follows algorithm in Section 6 of RFC3986.
- Port Normalization: Omit port subcomponent if equal to the scheme's default port.
- Path Normalization: Empty path component is equivalent to "/" in most cases.
- Case Sensitivity: Scheme and host are case-insensitive; other components are case-sensitive.
- Character Encoding: Characters not in the "reserved" set are equivalent to their percent-encoded octets.
- Example: Provides three equivalent URIs demonstrating normalization rules.

Certainly. Here's a combined summary with ABNF examples and real-world examples:

## 3. Message Format

HTTP/1.1 messages consist of:
- A start-line
- Zero or more header fields
- An empty line (CRLF)
- An optional message body

ABNF:
```abnf
HTTP-message   = start-line
                 *( header-field CRLF )
                 CRLF
                 [ message-body ]
```

Example:
```
GET /index.html HTTP/1.1
Host: www.example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8

```

Parsing procedure:
1. Read start-line into a structure
2. Read header fields into a hash table
3. Determine if a message body is expected
4. Read message body as a stream if present

Key points:
- Recipients must parse HTTP messages as a sequence of octets in US-ASCII superset encoding
- Incremental processing is possible but not guaranteed
- No whitespace allowed between start-line and first header field

### 3.1. Start Line

Two types of HTTP messages:
1. Request from client to server
2. Response from server to client

Differences:
- Start-line format (request-line vs. status-line)
- Algorithm for determining message body length

#### 3.1.1. Request Line

ABNF:
```abnf
request-line   = method SP request-target SP HTTP-version CRLF
```

Example:
```
GET /index.html HTTP/1.1
```

Key points:
- Method token is case-sensitive
- No predefined limit on request-line length
- Recommended minimum support: 8000 octets

#### 3.1.2. Status Line

ABNF:
```abnf
status-line = HTTP-version SP status-code SP reason-phrase CRLF
```

Example:
```
HTTP/1.1 200 OK
```

Key points:
- Status-code is a 3-digit integer
- Reason-phrase is optional and should be ignored by clients

### 3.2. Header Fields

ABNF:
```abnf
header-field   = field-name ":" OWS field-value OWS
```

Example:
```
Host: www.example.com
Content-Type: text/html; charset=utf-8
```

Key points:
- Field names are case-insensitive
- Field values may contain field content or obsolete line folding

#### 3.2.1. Field Extensibility

- Header fields are fully extensible
- Proxies must forward unrecognized header fields unless specified otherwise

Example of a custom header:
```
X-Custom-Header: Some-Value
```

#### 3.2.2. Field Order

- Order of different field names is not significant
- Multiple fields with the same name should be combined, maintaining order

Example of combining multiple fields:
```
Cache-Control: max-age=3600
Cache-Control: must-revalidate
```
Becomes:
```
Cache-Control: max-age=3600, must-revalidate
```

#### 3.2.3. Whitespace

Three types of whitespace rules:
1. OWS (optional whitespace)
2. RWS (required whitespace)
3. BWS ("bad" whitespace)

ABNF:
```abnf
OWS            = *( SP / HTAB )
RWS            = 1*( SP / HTAB )
BWS            = OWS
```

Example with OWS:
```
Host:   www.example.com
```

#### 3.2.4. Field Parsing

- Generic parsing algorithm, independent of field names
- No whitespace allowed between field-name and colon
- Deprecates line folding except within message/http media type

Example of invalid whitespace (which should be rejected):
```
Host : www.example.com
```

#### 3.2.5. Field Limits

- No predefined limit on header field length or total header section length
- Servers must respond with 4xx status code if received fields are too large

Example of a very long header that might be rejected:
```
X-Long-Header: This is a very long header value that goes on and on and might exceed the server's limit...
```

#### 3.2.6. Field Value Components

Common syntax components:
- token
- quoted-string
- comment

ABNF:
```abnf
token          = 1*tchar
quoted-string  = DQUOTE *( qdtext / quoted-pair ) DQUOTE
comment        = "(" *( ctext / quoted-pair / comment ) ")"
```

Examples:
```
Accept: text/html, application/xhtml+xml, application/xml;q=0.9, */*;q=0.8
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36
Set-Cookie: session_id="abc123"; Path=/; HttpOnly
```

Key points:
- Delimiters chosen from US-ASCII visual characters
- Backslash can be used as a quoting mechanism

This combined summary provides a comprehensive overview of the HTTP message format, including ABNF definitions, real-world examples, and key points for each section, offering a clear understanding of both the structure and practical application of HTTP messages.

