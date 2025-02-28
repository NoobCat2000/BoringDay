# BoringDay

Cái project nhỏ này là mình đánh cắp từ project PowerChell của Orange Cyberdefense. Vô tình lướt được ở trên twitter, ban đầu đọc README của project thì mình nghĩ nó chỉ là phiên bản vớ vẩn nào đó dùng CLR Hosting để chạy Powershell. Nhưng khi đọc code thì nhận ra nó là cái gì đó rất mới. Mọi người nên đọc blog gốc tại đây:
https://blog.scrt.ch/2025/02/18/reinventing-powershell-in-c-c/

Ngày trước thì Cobalt Strike có command là **execute-assembly** để thực thi được .NET Assembly trong unmanaged process. Bản chất của kỹ thuật đó là CLR Hosting + Load Assembly. Cơ mà sau này CLR có log cái sự kiện Assembly Load này qua ETW, sau đó EDR hoặc AV đọc từ ETW và có thể trích xuất được Assembly là scan có phải mã độc hay không.
Còn cái PowerChell này nó mới ở chỗ là sau khi CLR Hosting, nó sẽ quá mô phỏng lại code của C# thông qua COM, dùng C/C++. Ví dụ như sau:

```cs
using Microsoft.PowerShell.ConsoleHost;

class Program
{
    public static void Main()
    {
        ConsoleShell.Start("Banner", "Help Text", "ls");
    }
}
```
Khi dùng COM để thực hiện được 
* Bước 1: Load Assembly **Microsoft.PowerShell.ConsoleHost**.
* Bước 2: Tìm kiếm Class ConsoleShell trong Assembly **Microsoft.PowerShell.ConsoleHost**.
* Bước 3: Tìm kiếm Method Start của Class ConsoleShell.
* Bước 4: Khởi tạo tham số cho Method Start.
* Bước 5: Gọi method Start

Đây là code mẫu để thực hiện bước 4 và bước 5
```c
if (!FindMethodInArray(pConsoleShellMethods, L"Start", 4, &pStartMethodInfo))
    goto exit;

InitVariantFromString(pwszBanner, &vtBannerText);
InitVariantFromString(pwszHelp, &vtHelpText);
InitVariantFromStringArray(ppwszArguments, dwArgumentCount, &vtArguments);

pStartArguments = SafeArrayCreateVector(VT_VARIANT, 0, 4);

lArgumentIndex = 0;
SafeArrayPutElement(pStartArguments, &lArgumentIndex, pvtRunspaceConfiguration);
lArgumentIndex = 1;
SafeArrayPutElement(pStartArguments, &lArgumentIndex, &vtBannerText);
lArgumentIndex = 2;
SafeArrayPutElement(pStartArguments, &lArgumentIndex, &vtHelpText);
lArgumentIndex = 3;
SafeArrayPutElement(pStartArguments, &lArgumentIndex, &vtArguments);

hr = pStartMethodInfo->Invoke_3(vtEmpty, pStartArguments, &vtResult);
EXIT_ON_HRESULT_ERROR(L"MethodInfo->Invoke_3", hr);
```

Mình có search các blog nói về cái này thì thấy chưa ai làm cái trò này bao giờ cả, mỗi tội kỹ thuật này chỉ mô phỏng lại code C# nhỏ thôi, chứ mấy cái project lớn như SharpHound thì dùng cách như Cobalt Strike cho khoẻ.
Sau khi mình test thì thấy cách dùng ConsoleShell hơi bất tiện:
* Đầu tiên thì Method Start thực thi lệnh ls xong thì nó sẽ hiện cho hẳn cái powershell cho mọi người gõ :D và méo exit luôn.
* Thứ hai là không thể gọi Method Start trong cùng một Thread 2 lần, mình debug thì nguyên nhân do không thể set name cho thread đó 2 lần đối với .NET Framework.
* Không thể thực hiện đối với GUI App, chỉ dùng cho Console App.

Do mấy cái lý do trên nên mình có đã đổi sang dùng **System.Management.Automation** thay vì ConsoleHost. Cơ mà khi muốn viết bằng COM thì nó sẽ phức tạp hơn rất nhiều. Để cho dễ hiểu thì mình sẽ cố gắng mô phỏng lại đoạn code sau bằng C:
```cs
using System.Management.Automation;

class Program
{
    public static void Main()
    {
        // command.txt là đoạn powershell muốn thực thi
        string script = "Get-Content command.txt | Invoke-Expression | Out-String"
        using (PowerShell ps = PowerShell.Create())
        {
            ps.AddScript(script);
            Collection<PSObject> results = ps.Invoke()
            Console.WriteLine(result.ToString());
        }
    }
}
```

PoC của mình thì 90% là đi ăn cắp, cơ mà lâu quá chưa share gì nên mình muốn share 10% công sức của mình
