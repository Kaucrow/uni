# Connection.Dispose method

Releases the connection resources and returns it to the pool via the registered handler.

```csharp
public void Dispose()
```

## Remarks

If a return handler was set via SetReturnHandler, it will be invoked to properly return the connection to the pool. Subsequent calls to Dispose() will have no effect.

## See Also

* class [Connection](../Connection.md)
* namespace [global](../../dbcomponent.md)

<!-- DO NOT EDIT: generated by xmldocmd for dbcomponent.dll -->
