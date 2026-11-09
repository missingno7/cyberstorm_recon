/* Plain object, immediately after the runtime traversal helpers. Working labels. */
extern void *runtimeModuleHead;
extern char runtimeModuleAnchor;
void InitializeRuntimeModuleHead(void)
{ runtimeModuleHead=&runtimeModuleAnchor; }
