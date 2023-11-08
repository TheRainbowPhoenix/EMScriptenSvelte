export const create = (monaco: any) => {
  // Create a simple web worker
  const workerScript = `
self.onmessage=function(e) {
    console.log("Worker: Receiving message", e.data);
    const textUntilPosition = e.data;
    const data = {
        suggestions: [
            {
                label: "Foobar",
                kind: 25,
                detail: "Details for completion",
                insertText: "Message from webworker"
            }
        ]
    };
    postMessage({id: e.data.id, data: data, success: true});
}
`;
  let blob;
  try {
    blob = new Blob([workerScript], { type: "application/javascript" });
  } catch (e) {
    // Backwards-compatibility
    blob = new ((window as any).BlobBuilder ||
      (window as any).WebKitBlobBuilder ||
      (window as any).MozBlobBuilder)();
    blob.append(workerScript);
    blob = blob.getBlob();
  }
  const worker = new Worker(
    (window.URL || window.webkitURL).createObjectURL(blob)
  );

  // Handle messages from web worker
  let id = 0;
  /** @type {Map<string, {resolve: ()=>void, reject: ()=>void}>} */
  const messageMap = new Map();
  worker.onmessage = function (e) {
    console.log("Main: Receiving message", e.data);
    const promise = messageMap.get(e.data.id);
    if (promise) {
      messageMap.delete(e.data.id);
      if (e.data.success) promise.resolve(e.data.data);
      else promise.reject(e.data.data);
    }
  };

  // Register a new language
  monaco.languages.register({ id: "c" });

  // Register a completion item provider that queries the web worker
  monaco.languages.registerCompletionItemProvider("hhk", {
    provideCompletionItems(
      model: any,
      position: any,
      context: any,
      token: any
    ) {
      const textUntilPosition = model.getValueInRange({
        startLineNumber: 1,
        startColumn: 1,
        endLineNumber: position.lineNumber,
        endColumn: position.column,
      });
      const currentId = ++id;
      const promise = new Promise((resolve, reject) => {
        messageMap.set(currentId, { resolve, reject });
      });
      worker.postMessage({ id: currentId, data: textUntilPosition });
      return promise;
    },
    resolveCompletionItem(model: any, position: any, item: any, token: any) {
      return item;
    },
  });

  return monaco;

  // Create a new monaco editor
  //   monaco.editor.create(document.getElementById("container"), {
  //     value:
  //       "// Example for a custom language with webworker\n// Try typing 'Foobar'\n",
  //     language: "hhk",
  //   });
};
