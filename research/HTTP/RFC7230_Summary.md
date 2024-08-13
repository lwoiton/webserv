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

   				  >             >
          UA =========== A =========== B - - - - - - C - - - - - - O
                  <             <

- Cache: A local store of previous response messages and the system controlling its storage, retrieval, and deletion.
- Cacheable Response: A response that a cache is allowed to store for future use.
- Cache Constraints: Limitations placed by clients or origin servers on when a cached response can be used.
- Cache Chain: The shortened request/response path when a cached response is available.
- Cache Architectures: Various configurations of caches deployed across the web (e.g., national hierarchies, collaborative systems).
