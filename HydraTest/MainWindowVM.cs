using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive.Linq;
using System.Windows;
using System.Windows.Media;
using Addle.Wpf.ViewModel;

namespace HydraTest
{
	public class MainWindowVM
	{
		HydraLibrary _library;
		IDisposable _timerSubscription;

		[VMProperty]
		readonly AutoProperty<Brush> _leftButtonFill = new AutoProperty<Brush>();
		[VMProperty]
		readonly AutoProperty<Brush> _leftTriggerFill = new AutoProperty<Brush>();
		[VMProperty]
		readonly AutoProperty<Brush> _rightButtonFill = new AutoProperty<Brush>();
		[VMProperty]
		readonly AutoProperty<Brush> _rightTriggerFill = new AutoProperty<Brush>();

		[VMProperty(IsWritable = true)]
		readonly AutoProperty<Visibility> _foo = new AutoProperty<Visibility>();

		[VMProperty]
		readonly AutoCommand<MainWindowVM, string> _start = new AutoCommand<MainWindowVM, string>(@this => @this.StartExecute());
		void StartExecute()
		{
			if (_library == null)
			{
				_library = new HydraLibrary();

				_timerSubscription = Observable.Timer(TimeSpan.Zero, TimeSpan.FromMilliseconds(33)).ObserveOnDispatcher().Subscribe(Tick);
			}
			else
			{
				_timerSubscription.Dispose();
				_library.Dispose();
				_library = null;
			}
		}

		void Tick(long time)
		{
			_leftButtonFill.Value = _library.LeftController.ButtonJustPressed() ? Brushes.LightBlue : Brushes.LightGray;
			_leftTriggerFill.Value = _library.LeftController.TriggerJustPressed() ? Brushes.LightBlue : Brushes.LightGray;
			_rightButtonFill.Value = _library.RightController.ButtonJustPressed() ? Brushes.LightBlue : Brushes.LightGray;
			//_rightTriggerFill.Value = _library.RightController.TriggerJustPressed() ? Brushes.LightBlue : Brushes.LightGray;

			if (_library.RightController.TriggerJustPressed())
			{
				_rightTriggerFill.Value = Brushes.LightBlue;
				_foo.Value = Visibility.Hidden;
            }
		}
	}
}
