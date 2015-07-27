using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Addle.Wpf.ViewModel;

namespace HydraTest
{
	static class Program
	{
		[STAThread]
		static void Main()
		{
			var app = new App();
			var window = new MainWindow { DataContext = AutoVMFactory.Create<MainWindowVM>() };
			app.Run(window);
		}
	}
}
