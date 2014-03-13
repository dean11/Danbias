using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using System.Threading;
using System.Timers;
using System.IO;

namespace StandAloneLauncher
{
    public partial class NoEdgeWindow : Form
    {
        System.Windows.Interop.StandaloneGameServerCLI gameServer;
        bool serverIsRunning = false;
        bool gameIsStarted = false;
        int totalTime = 0;

        public NoEdgeWindow()
        {
            InitializeComponent();

            this.panel_serverOptions.MouseDown += NoEdgeWindow_MouseDown;
            //this.Size = this.panel_serverOptions.Size;
            this.panel_clientArea.Visible = false;
            this.Width = this.panel_serverOptions.Width;

#if (DEBUG)
           
#endif

            string[] maps = Directory.GetFiles("..\\Content\\Worlds\\");
            for (int i = 0; i < maps.Length; i++)
            {
                string temp = maps[i].Split('\\').Last();
                string type = temp.Split('.').Last();
                if (type == "bias")
                {
                    this.mapName.Items.Add(temp);
                }
            }

            this.gameModes.SelectedIndex = 0;
            this.mapName.SelectedIndex = 0;
        }

        public bool Initiate()
        {
            this.gameServer = new StandaloneGameServerCLI();

            return true;
        }

        void ServerFrame()
        {
            if (this.serverIsRunning)
            {
                this.gameServer.ServerUpdate();
                this.textBox_clients.Text = this.gameServer.GetClientsConnectedCount().ToString() + "/" + this.textBox_clientLimit.Text;
                int tot = this.gameServer.GameGetGameTime();
                int sec = tot % 60;
                int min = tot / 60 % 60;
                this.textBox_timeLeft.Text = min.ToString() + "." + sec.ToString();
            }
        }
     
        public void Run()
        {
            while (this.Created)
            {
                Application.DoEvents();

                ServerFrame();
            }
        }

        private bool InitServer()
        {
            ServerInitDesc desc = new ServerInitDesc();
            //desc.mainOptions.broadcast = this.lanBroadcast.Checked;
            desc.mainOptions.listenPort = Convert.ToInt32(this.textBox_TCPport.Text);
            desc.mainOptions.serverName = this.serverName.Text;

            if (this.gameServer.ServerInitiate(desc) == DanBiasServerReturn.DanBiasServerReturn_Sucess)
            {
                this.serverIsRunning = true;

                GameServerInfo info = this.gameServer.ServerGetInfo();
                this.textBox_name.Text = this.serverName.Text;
                this.textBox_ip.Text = info.serverIp + ": " + this.textBox_TCPport.Text;

                this.gameServer.ServerStart();
                this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Server initiated!\n\tListening on port " + this.textBox_TCPport.Text + "\n\tLocal IP: " + info.serverIp + "\n");
            }
            else
            {
                this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Failed to initiate the server!\n");
                return false;
            }
            return true;
        }

        private void buttonStartGame_Click(object sender, EventArgs e)
        {
            if (InitServer())
            {
                //this.gameServer.GameSetGameMode(this.gameModes.SelectedText);
                this.gameServer.GameSetGameTime(Convert.ToInt32(this.textBox_timelimit.Text));
                this.gameServer.GameSetMapName(this.mapName.Text);
                this.gameServer.GameSetMaxClients(Convert.ToInt32(this.textBox_clientLimit.Text));

                if (!(gameIsStarted = this.gameServer.GameStart(this.forceStart.Checked)))
                {
                    this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Failed to start the game session!\n");
                }
                else
                {
                    this.timerTotal.Start();
                    this.panel_serverOptions.Visible = false;
                    this.panel_clientArea.Visible = true;
                    this.Size = this.panel_clientArea.Size;
                }
                this.Width = this.panel_right.Width;
                this.Height = this.panel_right.Height;
            }
        }

        private void FormClosingEvent(object sender, FormClosingEventArgs e)
        {
            if (serverIsRunning)
            {
                this.gameServer.ServerStop();
            }
        }

    

        private void mapName_SelectedIndexChanged(object sender, EventArgs e)
        {
            //if ( this.panel_serverOptions.Visible && (this.mapName.SelectedItem.ToString() == "Set directory"))
            //{
            //    FolderBrowserDialog f = new FolderBrowserDialog();
            //    DialogResult r = f.ShowDialog();
            //    if (r == System.Windows.Forms.DialogResult.OK)
            //    {
            //        this.mapName.Items.Clear();
            //        this.mapName.Items.Add("Set directory");
            //        string[] maps = Directory.GetFiles(f.SelectedPath);
            //        for (int i = 0; i < maps.Length; i++)
            //        {
            //            string temp = maps[i].Split('\\').Last();
            //            string type = temp.Split('.').Last();
            //            if (type == "bias")
            //            {
            //                this.mapName.Items.Add(temp);
            //            }
            //        }
            //    }
            //}
        }

        private void textBox_consonl_textfield_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                if (this.textBox_consonl_textfield.Text.Length > 0)
                {
                    switch (this.textBox_consonl_textfield.Text)
                    {
                        case "exit":
                            this.Close();
                        return;

                        case "status":

                        break;
                    }
                    this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "-\t" + this.textBox_consonl_textfield.Text + "\n");
                    this.textBox_consonl_textfield.Text = "";
                }
            }
        }

        private void button_consol_excexute_Click(object sender, EventArgs e)
        {
            KeyPressEventArgs be = new KeyPressEventArgs('\r');
            textBox_consonl_textfield_KeyPress( null,  be);
        }

        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HTCAPTION = 0x2;
        [DllImport("User32.dll")]
        public static extern bool ReleaseCapture();
        [DllImport("User32.dll")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        private void NoEdgeWindow_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                ReleaseCapture();
                SendMessage(Handle, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            }
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void textboxKeyDownEvent(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.Back) return;
            int t;
            string c = ((char)e.KeyValue).ToString();
            if (!int.TryParse(c, out t))
            {
                e.SuppressKeyPress = true;
            }
        }

        private void button_rightMinimize_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }

        private void button_serverOnclose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void timerTotal_Tick(object sender, EventArgs e)
        {
            totalTime += 1;
            int sec = totalTime % 60;
            int min = totalTime / 60 % 60;
            this.textBox_timeTotal.Text = min.ToString() + "." + sec.ToString();
        }
        
    }
}
