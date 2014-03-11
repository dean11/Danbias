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
        bool draging = false;

        public NoEdgeWindow()
        {
            InitializeComponent();

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
                this.textBox_clients.Text = this.gameServer.GetClientsConnectedCount().ToString() + "/" + ((int)this.nrOfClients.Value).ToString();
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
            desc.mainOptions.listenPort = (int)this.listenPort.Value;
            desc.mainOptions.serverName = this.serverName.Text;

            if (this.gameServer.ServerInitiate(desc) == DanBiasServerReturn.DanBiasServerReturn_Sucess)
            {
                this.serverIsRunning = true;

                GameServerInfo info = this.gameServer.ServerGetInfo();
                this.textBox_name.Text = this.serverName.Text;
                this.textBox_ip.Text = info.serverIp + ": " + this.listenPort.Value.ToString();

                this.listenPort.Enabled = false;
                this.serverName.Enabled = false;
                this.gameServer.ServerStart();
                this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Server initiated!\n\tListening on port " + this.listenPort.Value.ToString() + "\n\tLocal IP: " + info.serverIp + "\n");
                //this.panelServerCommands.Visible = true;
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
                this.gameServer.GameSetGameTime((int)this.timeLimit.Value);
                this.gameServer.GameSetMapName(this.mapName.Text);
                this.gameServer.GameSetMaxClients((int)this.nrOfClients.Value);

                if (!(gameIsStarted = this.gameServer.GameStart(this.forceStart.Checked)))
                {
                    this.ServerInfoTextArea.AppendText(DateTime.Now.ToUniversalTime() + "\n\t" + "Failed to start the game session!\n");
                }
                else
                {
                    this.panel_serverOptions.Visible = false;
                    this.panel_clientArea.Visible = true;
                    this.Size = this.panel_clientArea.Size;
                }
                this.Width = this.panel_serverOptions.Width;
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

        private void NoEdgeWindow_MouseDown(object sender, MouseEventArgs e)
        {
            this.draging = true;
        }

        private void NoEdgeWindow_MouseUp(object sender, MouseEventArgs e)
        {
            this.draging = false;
        }

        private void NoEdgeWindow_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.draging)
            {

            }
        }
    }
}
