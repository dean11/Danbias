using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EditorUI
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new NoEdge());

            NoEdge app = new NoEdge();
            app.Show();
            if (app.Initiate())
            {
                app.Run();
            }
        }
    }
}
