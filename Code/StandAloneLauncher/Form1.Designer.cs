namespace StandAloneLauncher
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.serverToggle = new System.Windows.Forms.Button();
            this.lanBroadcast = new System.Windows.Forms.CheckBox();
            this.serverName = new System.Windows.Forms.TextBox();
            this.label_serverName = new System.Windows.Forms.Label();
            this.listenPort = new System.Windows.Forms.NumericUpDown();
            this.label_listenPort = new System.Windows.Forms.Label();
            this.panel_serverOptions = new System.Windows.Forms.Panel();
            this.panel_commands = new System.Windows.Forms.Panel();
            this.panel_clientArea = new System.Windows.Forms.Panel();
            this.clientInfoBox = new System.Windows.Forms.ListBox();
            ((System.ComponentModel.ISupportInitialize)(this.listenPort)).BeginInit();
            this.panel_serverOptions.SuspendLayout();
            this.panel_clientArea.SuspendLayout();
            this.SuspendLayout();
            // 
            // serverToggle
            // 
            this.serverToggle.Location = new System.Drawing.Point(90, 81);
            this.serverToggle.Name = "serverToggle";
            this.serverToggle.Size = new System.Drawing.Size(75, 23);
            this.serverToggle.TabIndex = 0;
            this.serverToggle.Text = "Start server";
            this.serverToggle.UseVisualStyleBackColor = true;
            this.serverToggle.Click += new System.EventHandler(this.button1_serverToggle_Click);
            // 
            // lanBroadcast
            // 
            this.lanBroadcast.AutoSize = true;
            this.lanBroadcast.Location = new System.Drawing.Point(81, 58);
            this.lanBroadcast.Name = "lanBroadcast";
            this.lanBroadcast.Size = new System.Drawing.Size(95, 17);
            this.lanBroadcast.TabIndex = 1;
            this.lanBroadcast.Text = "Lan Broadcast";
            this.lanBroadcast.UseVisualStyleBackColor = true;
            // 
            // serverName
            // 
            this.serverName.Location = new System.Drawing.Point(81, 3);
            this.serverName.Name = "serverName";
            this.serverName.Size = new System.Drawing.Size(95, 20);
            this.serverName.TabIndex = 3;
            // 
            // label_serverName
            // 
            this.label_serverName.AutoSize = true;
            this.label_serverName.Location = new System.Drawing.Point(6, 6);
            this.label_serverName.Name = "label_serverName";
            this.label_serverName.Size = new System.Drawing.Size(69, 13);
            this.label_serverName.TabIndex = 4;
            this.label_serverName.Text = "Server Name";
            // 
            // listenPort
            // 
            this.listenPort.Location = new System.Drawing.Point(81, 29);
            this.listenPort.Maximum = new decimal(new int[] {
            65000,
            0,
            0,
            0});
            this.listenPort.Minimum = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            this.listenPort.Name = "listenPort";
            this.listenPort.Size = new System.Drawing.Size(95, 20);
            this.listenPort.TabIndex = 5;
            this.listenPort.Value = new decimal(new int[] {
            2048,
            0,
            0,
            0});
            // 
            // label_listenPort
            // 
            this.label_listenPort.AutoSize = true;
            this.label_listenPort.Location = new System.Drawing.Point(6, 31);
            this.label_listenPort.Name = "label_listenPort";
            this.label_listenPort.Size = new System.Drawing.Size(56, 13);
            this.label_listenPort.TabIndex = 4;
            this.label_listenPort.Text = "Listen port";
            // 
            // panel_serverOptions
            // 
            this.panel_serverOptions.Controls.Add(this.serverName);
            this.panel_serverOptions.Controls.Add(this.listenPort);
            this.panel_serverOptions.Controls.Add(this.serverToggle);
            this.panel_serverOptions.Controls.Add(this.label_listenPort);
            this.panel_serverOptions.Controls.Add(this.lanBroadcast);
            this.panel_serverOptions.Controls.Add(this.label_serverName);
            this.panel_serverOptions.Location = new System.Drawing.Point(12, 12);
            this.panel_serverOptions.Name = "panel_serverOptions";
            this.panel_serverOptions.Size = new System.Drawing.Size(183, 112);
            this.panel_serverOptions.TabIndex = 6;
            // 
            // panel_commands
            // 
            this.panel_commands.Location = new System.Drawing.Point(12, 130);
            this.panel_commands.Name = "panel_commands";
            this.panel_commands.Size = new System.Drawing.Size(183, 231);
            this.panel_commands.TabIndex = 7;
            // 
            // panel_clientArea
            // 
            this.panel_clientArea.Controls.Add(this.clientInfoBox);
            this.panel_clientArea.Location = new System.Drawing.Point(202, 12);
            this.panel_clientArea.Name = "panel_clientArea";
            this.panel_clientArea.Size = new System.Drawing.Size(303, 349);
            this.panel_clientArea.TabIndex = 8;
            // 
            // clientInfoBox
            // 
            this.clientInfoBox.FormattingEnabled = true;
            this.clientInfoBox.Location = new System.Drawing.Point(3, 6);
            this.clientInfoBox.Name = "clientInfoBox";
            this.clientInfoBox.Size = new System.Drawing.Size(297, 342);
            this.clientInfoBox.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(517, 373);
            this.Controls.Add(this.panel_clientArea);
            this.Controls.Add(this.panel_commands);
            this.Controls.Add(this.panel_serverOptions);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.listenPort)).EndInit();
            this.panel_serverOptions.ResumeLayout(false);
            this.panel_serverOptions.PerformLayout();
            this.panel_clientArea.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button serverToggle;
        private System.Windows.Forms.CheckBox lanBroadcast;
        private System.Windows.Forms.TextBox serverName;
        private System.Windows.Forms.Label label_serverName;
        private System.Windows.Forms.NumericUpDown listenPort;
        private System.Windows.Forms.Label label_listenPort;
        private System.Windows.Forms.Panel panel_serverOptions;
        private System.Windows.Forms.Panel panel_commands;
        private System.Windows.Forms.Panel panel_clientArea;
        private System.Windows.Forms.ListBox clientInfoBox;
    }
}

