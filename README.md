# CAgents

A tiny, native, distributed runtime for AI agents, written in C. Python, Rust, Go (or anything that speaks a socket) connect as clients.

> **Status: pre-alpha / design phase.** This README describes the target design.

## Idea

CAgents is a small C core that owns **scheduling, messaging, state, persistence and distribution**. Agent logic (LLM calls, tools) lives in client processes. You describe your workflow as a **graph**, and the runtime executes it.

```
  Python / Rust / Go clients      ← node logic: LLMs, tools
            │  binary protocol (Unix socket / TCP)
  ┌─────────▼──────────┐
  │   cagents node     │  graph executor · scheduler · event loop
  │                    │  mailboxes · WAL + snapshots
  └─────────┬──────────┘
            │  node-to-node
      other cagents nodes
```

## Graph orchestration

Workflows are directed graphs, similar to LangGraph:

- **Nodes** are agents or steps, implemented by a client.
- **Edges** route messages between nodes, either fixed or conditional (the node's output picks the next edge).
- **Cycles** are allowed (loops, retries, reflection).
- **Fan-out / fan-in** runs branches in parallel and joins their results.
- **Shared graph state** is updated by nodes and checkpointed after every step.
- **Resume and replay**: a crashed or paused run continues from its last checkpoint.
- **Human-in-the-loop**: a run can pause on a node until an external message arrives.

The graph is defined by the client (as data, sent over the protocol); the runtime schedules nodes, routes messages and persists state. Nodes may execute on different clients or machines.

```python
from cagents import Client, Graph

g = Graph("research")
g.node("plan", planner)
g.node("search", searcher)
g.node("write", writer)

g.edge("plan", "search")
g.edge("search", "write", when=lambda s: s["enough_sources"])
g.edge("search", "plan",  when=lambda s: not s["enough_sources"])  # loop

client = Client.connect("unix:///tmp/cagents.sock")
run = client.run(g, input={"topic": "..."})
print(run.result())
```

## Design

- **Execution:** thread-per-core scheduler with work stealing; `epoll` event loop for I/O.
- **Memory:** per-run arenas and slab pools; bounded queues and explicit limits.
- **Persistence:** write-ahead log plus snapshots, with crash recovery.
- **Fault tolerance:** timeouts, retries and restart strategies per node.
- **Distribution:** gossip membership; runs and nodes are placed and routed across the cluster.
- **Protocol:** length-prefixed, versioned binary frames; payloads are opaque to the runtime.

## Non-goals

Running LLMs, Windows support, being a general-purpose actor framework.

## Build

Requires Linux and a C99 compiler

## License

MIT