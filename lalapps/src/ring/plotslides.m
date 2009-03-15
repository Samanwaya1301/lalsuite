function plotslides( ifos )

% 
% NULL = plotslides ( ifos )
% 
% Generate plots of double coincident background events of all combinations of input ifos, 
% where ifos is a column vector of two character ifo labels: 'H1', 'H2', 'L1', 'V1', 'G1'. 
% 
% plotslides will print a message if there are triple coincidenct background events
%

N_ifos = length( ifos(:,1) );
TRIP = 'triples exist in ';

for i=1:N_ifos
  if exist(['bg',ifos(i,:),'_trip.mat'],'file')
    TRIP=[TRIP,ifos(i,:)];
  end
end
if ( length(TRIP) > 20 )
  TRIP = [TRIP,' time.']
end

for i=1:N_ifos
  for j=2:N_ifos
    if (i < j)
      bg1in2=load(['bg',ifos(i,:),'in',ifos(j,:),'_doub.mat']);
      bg2in1=load(['bg',ifos(j,:),'in',ifos(i,:),'_doub.mat']);

      for k=1:length(bg1in2.id)
        tmp=bg1in2.id(k);
        alsotmp=tmp{1};
        bg1in2.slidenum(k)=str2num(alsotmp(33:36));
      end

      for k=1:length(bg1in2.slidenum)
        if bg1in2.slidenum(k)>5000 % negative timeslide
          event(k)=(bg1in2.slidenum(k)-5000)*-1;
        else                          % positive timeslide
          event(k)=bg1in2.slidenum(k);
        end
      end
      
      % plot number events per slide
      figure
      hist(event,101)
      hold on
      h=findobj(gca,'Type','patch');
      set(h,'FaceColor','r')  %set the time-slides to red
      [A,B]=hist(event,101);
      ave=sum(A)/100;
      plot([-60,60],[ave,ave],'k--')
      h_xlab=xlabel('SlideNumber');
      h_ylab=ylabel('Number of Coincidences');
      h_t=title( ['Number of background events in ', ifos(i,:), ifos(j,:)] );
      set(h_xlab,'FontSize',16);
      set(h_ylab,'FontSize',16);
      set(h_t,'FontSize',16);
      grid on
      axis([-60 60 0 320])
      set(gca,'FontSize',16);
      axis autoy          
      saveas(gcf,[ifos(i,:),ifos(j,:),'bghist.png'])
      ave_num_bg = ave

      % plot number events per frequency bin
      figure
      hist(log10(bg1in2.f),100)
      h_xlab=xlabel('log_{10}(f)');
      h_ylab=ylabel('Number of Coincidences');
      h_t=title(['Number of background events in ',ifos(i,:),ifos(j,:),'as a function of frequency']);
      set(gca,'FontSize',16);
      grid on
      set(h_xlab,'FontSize',16);
      set(h_ylab,'FontSize',16);
      set(h_t,'FontSize',16);
      saveas(gcf,[ifos(i,:),ifos(j,:),'bgfhist.png'])

      % plot dt v. f for all events
      dt = mod( (bg1in2.t-bg2in1.t), 1 ); % reslide data
      dt = (dt - (dt > .5))*1.e3; % adjust for t2>t1 and convert to s -> ms
      figure
      semilogx(bg1in2.f,dt,'b+')
      xlabel('f (Hz)')
      ylabel('dt (ms)')
      title(['dt v. f for ', ifos(i,:), ifos(j,:), ' background events'])
      grid on
      saveas(gcf,[ifos(i,:), ifos(j,:), 'bgdtvf.png'])

    end
  end
end
