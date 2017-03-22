package com.example.client;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.os.Handler;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import android.os.AsyncTask;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;


public class MainActivity extends ActionBarActivity {
	TextView textRecieve;
	EditText editTextAddress, editTextPort,textSend; 
	Button buttonConnect, buttonClear;
	
	int port; 
    
 
    String serverIpAddress = "";
 
 boolean connected = false;
 Handler handler = new Handler();
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		

	
		
			
			
			editTextAddress = (EditText)findViewById(R.id.address);
			editTextPort = (EditText)findViewById(R.id.port);
			buttonConnect = (Button)findViewById(R.id.connect);
           textRecieve=(TextView)findViewById(R.id.recieve);
			buttonClear = (Button)findViewById(R.id.clear);
			textSend= (EditText)findViewById(R.id.send);
			
			buttonConnect.setOnClickListener(buttonConnectOnClickListener);
			
			buttonClear.setOnClickListener(new OnClickListener(){

				@Override
				public void onClick(View v) {
					editTextAddress.setText("");
					editTextPort.setText("");
				}});
		}
		
		OnClickListener buttonConnectOnClickListener = new OnClickListener(){

					public void onClick(View arg0) {
						
					     serverIpAddress = editTextAddress.getText().toString();
					     
					     port = Integer.parseInt(editTextPort.getText().toString());
					     
			                    Thread cThread = new Thread(new ClientThread());
			                    cThread.start();
					    
					}};    
					public class ClientThread implements Runnable {
						 
						 public void run() {
					            try {
					                InetAddress serverAddr = InetAddress.getByName(serverIpAddress);
					                Log.d("ClientActivity", "C: Connecting...");
					                Socket socket = new Socket(serverAddr, port);
					                connected = true;
					               
					                PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket
		                                    .getOutputStream())), true);
					                while (connected) {
					                    try {
					                        Log.d("ClientActivity", "C: Sending command.");
					                        
					                    	ByteArrayOutputStream byteArrayOutputStream = 
					        		                new ByteArrayOutputStream(1024);
					        				byte[] buffer = new byte[1024];
					        				
					        				int bytesRead;
					        				String response = "";
					        				InputStream inputStream = socket.getInputStream();
					        				
					        				/*
					        				 * notice:
					        				 * inputStream.read() will block if no data return
					        				 */
					        	            while ((bytesRead = inputStream.read(buffer)) != -1){
					        	                byteArrayOutputStream.write(buffer, 0, bytesRead);
					        	                
					        	                response+= byteArrayOutputStream.toString("UTF-8");
					        	                textRecieve.setText(response);
					        	                out.println(textSend.getText().toString());
					        	                }
					                           
					                            Log.d("ClientActivity", "C: Sent.");
					                    } catch (Exception e) {
					                        Log.e("ClientActivity", "S: Error", e);
					                    }
					                }
					                socket.close();
					                Log.d("ClientActivity", "C: Closed.");
					            } catch (Exception e) {
					                Log.e("ClientActivity", "C: Error", e);
					                connected = false;
					            }
					        }				    }
}
					