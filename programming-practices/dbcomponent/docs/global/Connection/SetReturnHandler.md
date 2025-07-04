# Connection.SetReturnHandler method

Sets the handler that will be invoked when the connection is returned to the pool.

```csharp
public void SetReturnHandler(Action handler)
```

| parameter | description |
| --- | --- |
| handler | The action to execute when returning the connection |

## Remarks

The handler is typically set by the connection pool manager to properly return the connection to the available pool.

## See Also

* class [Connection](../Connection.md)
* namespace [global](../../dbcomponent.md)

<!-- DO NOT EDIT: generated by xmldocmd for dbcomponent.dll -->
